#include "heaplib.h"
//#include "mem.h"
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr))+(num_bytes))
#define SUB_BYTES(base_addr, num_bytes) (((char *)(base_addr))-(num_bytes))

//#define PRINT_DEBUG

//struct for heap header 
typedef struct _heapMeta{
	unsigned int contigFree; // the largest contiguous memory available
	unsigned int heapSize;
	unsigned int alingment;
}heapMeta;


//struct for block header and footer (both are identical)
typedef struct _blockMeta{
	unsigned int size;//size of the block including header and footer last three bits indicate if block is in use.
}blockMeta;


// start of helper functions alloc and release with no lock. they will be inline functions. 

void *hl_alloc_no_lock(void *heap, unsigned int block_size) {
	#ifdef PRINT_DEBUG
		printf("Entering alloc \n");
	#endif
	//find the "best" place in the heap to put this block, and update the block info at the beginning of the heap

	heapMeta *head;
	unsigned int heapInt = (unsigned int)(heap);
	if(heapInt%8 ==0){
		head = (heapMeta *)heap;
	}
	else{
		head = (heapMeta *)(ADD_BYTES(heap, 8-(heapInt%8)));
	}
	#ifdef PRINT_DEBUG
		printf("head= %p\n", head);
	#endif
	blockMeta *blockHead = (blockMeta *)(ADD_BYTES(head, sizeof(heapMeta)));
	blockMeta *blockFoot = (blockMeta *)(ADD_BYTES(blockHead, blockHead->size -4));
	
	//checks if the block asked for is greater than the largest contig mem available in the heap.
	#ifdef PRINT_DEBUG
		printf("contigFree %u\n", head->contigFree);
		printf("block_size %u\n", block_size);
	#endif
	if(block_size%8==0){
		if((head->contigFree ) < block_size){
			return NULL;
		}
	}
	else{
		if((head->contigFree ) < block_size+8-(block_size%8)){
			return NULL;
		}		
	}
	//optimazation goal is to put requested block in free block with least left over bytes.
	
	#ifdef PRINT_DEBUG
		printf("befor block search \n");
	#endif	
	
	unsigned int minLeftOver = head->contigFree;
	blockMeta *bestHead = blockHead;

	
	// added the -8 at the end to take care of cases where there is a stranded 1-7 bytes at the end which are not enough for block meta
	while(blockHead <= (blockMeta *)(ADD_BYTES(head, head->heapSize -8))){
		if(((blockHead->size)%2 == 0) &&(((blockHead->size)-8) >= block_size )){
			//i moved the block_size mod eight one level out because the if statements comparing to the minLeftOver need to be different
			if(block_size %8 ==0){
				if((blockHead->size - block_size - (2*(sizeof(blockMeta))))<minLeftOver){
					bestHead = blockHead;
					minLeftOver = blockHead->size - block_size - (2*(sizeof(blockMeta)));
				}
			}
			else{
				if((blockHead->size - block_size +8 - (block_size%8)- (2*(sizeof(blockMeta))))<minLeftOver){
					bestHead = blockHead;
					minLeftOver = blockHead->size - block_size + 8 - (block_size%8) - (2*(sizeof(blockMeta)));
				}
			}
		}
		blockHead = (blockMeta *)(ADD_BYTES(blockHead, (blockHead->size -(blockHead->size %2)))); 
	} 
	//first free implementation
	//while(((((blockHead->size)%2) != 0) || (((blockHead->size)-8) < block_size)) && (ADD_BYTES(blockHead, (block_size + block_size%8)) < ADD_BYTES(head,(head->heapSize)))){
		//blockHead = (blockMeta *)(ADD_BYTES(blockHead, (blockHead->size -(blockHead->size %2)))); 
	//}
	blockHead = bestHead;
	if((blockHead > (blockMeta *)(ADD_BYTES(head, head->heapSize -8)))||(blockHead->size%2==1)){
		return NULL;
	}
	// storing old blockhead pointer and size to temps
	blockMeta *oldPointer= blockHead;
	unsigned int oldSize = blockHead->size;
	//change blockhead size to new block size+1 plus space for header and footer
	if(block_size%8 != 0){
		blockHead->size = block_size- (block_size%8) + 8 +(sizeof(blockMeta))*2 +1;
	}
	else{
		blockHead->size = block_size +(sizeof(blockMeta))*2 +1;
	}
	//make new block footer
	blockFoot = (blockMeta *)((ADD_BYTES(blockHead, blockHead->size - ((blockHead->size)%2) -sizeof(blockMeta))));
	#ifdef PRINT_DEBUG
		printf("New Foot= %p\n", blockFoot);
	#endif
	blockFoot->size = blockHead->size;
	// only need to make new header and update footer for left over if the sizes dont match up perfectly 
	if((blockMeta *)(ADD_BYTES(blockFoot,3*(sizeof(blockMeta)))) <= (blockMeta *)(ADD_BYTES(oldPointer, oldSize))){
		//make new header for leftover block 
		blockHead = (blockMeta *)(ADD_BYTES(blockFoot, sizeof(blockMeta)));
		#ifdef PRINT_DEBUG
			//printf("head for free block = %p\n", blockHead);
		#endif
		if(block_size%8 != 0){
			blockHead->size = oldSize - block_size -2*(sizeof(blockMeta)) + (block_size%8) - 8;
			#ifdef PRINT_DEBUG
				//printf("blockhead size=  %u\n", blockHead->size);
			#endif
		}
		else{
			blockHead->size = oldSize - block_size -2*(sizeof(blockMeta));
			#ifdef PRINT_DEBUG
				//printf("blockhead size=  %u\n", blockHead->size);
			#endif
		}
		#ifdef PRINT_DEBUG
			//printf("block head for free block size = %d\n", blockHead->size);
		#endif
		//update old footer
		blockFoot = (blockMeta *)(ADD_BYTES(blockHead, ((blockHead->size) - sizeof(blockMeta))));
		#ifdef PRINT_DEBUG
			//printf("foot for free block = %p\n", blockFoot);
		#endif
		blockFoot->size = blockHead->size;
	}
	#ifdef PRINT_DEBUG
		printf("first matching block pointer = %p\n", (void *)(ADD_BYTES(oldPointer,sizeof(blockMeta))));
	#endif
	void *newPointer = (void *)(ADD_BYTES(oldPointer,sizeof(blockMeta)));
	
	//update contig free
	blockHead = (blockMeta *)(ADD_BYTES(head, sizeof(heapMeta)));
	blockFoot = (blockMeta *) (ADD_BYTES(blockHead, blockHead->size  -((blockHead->size)%2 - 4)));
	unsigned int maxContig =0;
	//maybe it is not good to compare block head but we should do something with foot instead before was just ADD_BYTES(blockHead, 8)
	while((blockHead <= (blockMeta *)(ADD_BYTES(head, head->heapSize -8)))&&(blockHead->size != 0)){
		if(((blockHead->size -8) > maxContig) && ((blockHead->size)%2==0)){
			maxContig = (blockHead->size) - 8;
			#ifdef PRINT_DEBUG
				//printf("in if \n");
			#endif
		}
		blockHead = (blockMeta *)(ADD_BYTES(blockHead, (blockHead->size)-((blockHead->size)%2)));
		//blockFoot = (blockMeta *) (ADD_BYTES(blockHead, blockHead->size  -((blockHead->size)%2 - 4)));
	}
	head->contigFree = maxContig;
	return newPointer;
	
}
/* Releases the block of previously allocated memory pointed to by
 * block (which currently resides in the heap pointed to by
 * heap). Acts as a NOP if block == 0.
 *
 * PRECONDITIONS:
 * (1) block must have been returned to the user from a prior
 * call to hl_alloc or hl_resize
 * (2) hl_release can only be called ONCE in association with that
 * prior call to hl_alloc or hl_resize
 * If preconditions are violated, non-graceful failure is acceptable.
 */
 
// acts like a nop if block == 0
void hl_release_no_lock(void *heap, void *block) {
	#ifdef PRINT_DEBUG
		printf("Entering release \n");
	#endif
	heapMeta *head;
	unsigned int heapInt = (unsigned int)(heap);
	if(heapInt%8 ==0){
		head = (heapMeta *)heap;
	}
	else{
		head = (heapMeta *)(ADD_BYTES(heap, 8-(heapInt%8)));
	}
	blockMeta *blockHead = (blockMeta *)(SUB_BYTES(block, 4));
	if(block == 0){
		return;
	}
	#ifdef PRINT_DEBUG
		printf("found head \n");
	#endif
	// find the block that we would like to free and change the inUse attribute to False;
	blockMeta *blockFoot = (blockMeta *)(ADD_BYTES(blockHead, (blockHead->size - ((blockHead->size)%2) -4)));
	#ifdef PRINT_DEBUG
		printf("changing to not in use\n");
		printf("blockHead  = %p\n",blockHead);
		printf("blockFoot = %p\n",blockFoot);  
		printf("blockHead size = %d\n", blockHead->size); 
		printf("blockFoot size = %d\n", blockFoot->size); 
	#endif
	
	blockHead->size = blockHead->size - ((blockHead->size)%2);
	blockFoot->size = blockFoot->size - ((blockFoot->size)%2);;
	#ifdef PRINT_DEBUG
		printf("checking if neighbors are free \n");
	#endif
	//checks if block above is free
	if((void *)(SUB_BYTES(blockHead,20)) >= (void *) (head)){
		#ifdef PRINT_DEBUG
			printf("checking if prev is free in if \n");
		#endif
		blockMeta *prevFoot = (blockMeta *)(SUB_BYTES(blockHead,4));
		blockMeta *prevHead = (blockMeta *)(SUB_BYTES(prevFoot, prevFoot->size -((prevFoot->size)%2)  -4));
		if((prevFoot->size)%2 ==0){
			prevHead->size = prevHead->size + blockHead->size;
			blockFoot->size = prevHead->size;
			blockHead = prevHead;
		}
	}
	//checks if block below is free
	if((void *)(ADD_BYTES(blockFoot,12)) <= (void *)(ADD_BYTES(head,head->heapSize))){
		#ifdef PRINT_DEBUG
			printf("checking if next is free in if \n");
		#endif
		blockMeta *nextHead = (blockMeta *)(ADD_BYTES(blockFoot,4));
		blockMeta *nextFoot = (blockMeta *)(ADD_BYTES(nextHead, nextHead->size - ((nextHead->size)%2)-4));
		if((nextHead->size)%2 ==0){
			blockHead->size = blockHead->size +nextHead->size;
			nextFoot->size = blockHead->size;
			blockFoot = nextFoot;
		}		
	}
	//update contig free
	#ifdef PRINT_DEBUG
		printf("old contig free = %u\n", head->contigFree);
	#endif
	blockHead = (blockMeta *)(ADD_BYTES(head, sizeof(heapMeta)));
	blockFoot = (blockMeta *) (ADD_BYTES(blockHead, blockHead->size  -((blockHead->size)%2 - 4)));
	unsigned int maxContig =0;
	//maybe it is not good to compare block head but we should do something with foot instead before was just ADD_BYTES(blockHead, 8)
	while((blockHead <= (blockMeta *)(ADD_BYTES(head, head->heapSize -8)))&&(blockHead->size != 0)){
		if(((blockHead->size -8) > maxContig) && ((blockHead->size)%2==0)){
			maxContig = (blockHead->size) - 8;
			#ifdef PRINT_DEBUG
				printf("in if \n");
			#endif
		}
		blockHead = (blockMeta *)(ADD_BYTES(blockHead, (blockHead->size)-((blockHead->size)%2)));
		//blockFoot = (blockMeta *) (ADD_BYTES(blockHead, blockHead->size  -((blockHead->size)%2 - 4)));
	}
	head->contigFree = maxContig;
    return;
}


//takes in a pointer and a size. sets up a heap beginning at *heap of size heap_size
/* Sets up a new heap beginning at 'heap' of size 'heap_size' (in
 * bytes).  This function does not allocate the heap (of size
 * heap_size) pointed to by heap. That should be done already by
 * the user calling this function.
 *
 * PRECONDITIONS:
 * (1) the allocated heap must be 'heap_size' in bytes
 * (2) heap_size must be >= MIN_HEAP_SIZE bytes.
 * If preconditions are violated, non-graceful failure is acceptable.
 */
 

void hl_init(void *heap, unsigned int heap_size) {
	spin_lock(malloc_lock);
	#ifdef PRINT_DEBUG
		printf("Entering init \n");
	#endif
	heapMeta *head;
	unsigned int heapInt = (unsigned int)(heap);
	if(heapInt%8 ==0){
		head = (heapMeta *)heap;
		head->heapSize = heap_size - heap_size%8;
	}
	else{
		head = (heapMeta *)(ADD_BYTES(heap, 8-(heapInt%8)));
		head->heapSize = (heap_size -8+(heapInt%8)) -((heap_size -8+(heapInt%8))%8);
	}
	
	
	blockMeta *blockHead = (blockMeta *)head;
	//blockhead increments by 4 bytes and we want to increment it by 8 bytes
	blockHead += sizeof(heapMeta)/sizeof(blockMeta);
	blockMeta *blockFoot = (blockMeta *)(ADD_BYTES(head, head->heapSize- 2*sizeof(blockMeta)));
	
	
	//assign heap meta to beginning of heap
	//initializing the rest of the heap to be one unused block (with a header and footer)
	blockHead->size = (head->heapSize - 16);
	blockFoot->size = (head->heapSize - 16);
	head->contigFree = blockHead->size -2*(sizeof(blockMeta));
	#ifdef PRINT_DEBUG
		//printf("Heap Size = %u\n", head->heapSize);
		//printf("Heap head pointer = %p\n", head);
		//printf("Max Contig Free = %u\n", head->contigFree);
		//printf("Heap block head meta = %u\n", blockHead->size);
		//printf("Heap block head pointer = %p\n", blockHead);
		//printf("Heap block foot meta = %u\n", blockFoot->size);
		//printf("Heap block foot pointer = %p\n", blockFoot);
	#endif
	spin_unlock(malloc_lock);
    return;
    
}

/* Allocates a block of memory of size block_size bytes from the heap starting
 * at 'heap'. Returns a pointer to the block on success; returns
 * 0 if the allocator cannot satisfy the request.
 *
 * PRECONDITIONS:
 * (1) hl_init must have been called exactly once for this heap
 * If preconditions are violated, non-graceful failure is acceptable.
 */
//allocated a block of memory of size block_size (bytes) from the heap
// returns a pointer to the block on success, returns 0 (NULL) if cannot satisfy the request
// can allocate more than the requested size but never less

void *hl_alloc(void *heap, unsigned int block_size) {
	spin_lock(malloc_lock);
	void *pointer = hl_alloc_no_lock(heap, block_size);
	spin_unlock(malloc_lock);
	return pointer;
}
/* Releases the block of previously allocated memory pointed to by
 * block (which currently resides in the heap pointed to by
 * heap). Acts as a NOP if block == 0.
 *
 * PRECONDITIONS:
 * (1) block must have been returned to the user from a prior
 * call to hl_alloc or hl_resize
 * (2) hl_release can only be called ONCE in association with that
 * prior call to hl_alloc or hl_resize
 * If preconditions are violated, non-graceful failure is acceptable.
 */
 
// acts like a nop if block == 0
void hl_release(void *heap, void *block) {
	spin_lock(malloc_lock);
	hl_release_no_lock(heap, block);
	spin_unlock(malloc_lock);
    return;
}


/* Changes the size of the block pointed to by block (that
 * currently resides in the heap pointed to by heap), from its
 * current size to size new_size, returning a pointer to the new
 * block, or 0 if the request cannot be satisfied. The contents of
 * the block should be preserved (even if the location of the
 * block changes -- this will happen when it is not possible to
 * increase the size of the current block but there is room
 * elsewhere on the heap to satisfy the request). If block == 0,
 * function should behave like hl_alloc().
 * PRECONDITIONS:
 * (1) block must have been returned to the user from a prior
 * call to hl_alloc or hl_resize
 * If preconditions are violated, non-graceful failure is acceptable.
 */

void *hl_resize(void *heap, void *block, unsigned int new_size) {
	spin_lock(malloc_lock);
	#ifdef PRINT_DEBUG
		printf("Entering resize \n");
	#endif
	//find the block that we wish to resize, check if there is contiguous space
	// if there is change the block size and we are set
	// if there is not space need to find new space big enough in heap, copy over the old values and free old block
	if(block == 0){
		void *blockZero = hl_alloc_no_lock(heap, new_size);
		spin_unlock(malloc_lock);
		return blockZero;
	}
	heapMeta *head;
	//checking for heap alignment at beginning of the heap
	unsigned int heapInt = (unsigned int)(heap);
	if(heapInt%8 ==0){
		head = (heapMeta *)heap;
	}
	else{
		head = (heapMeta *)(ADD_BYTES(heap, 8-(heapInt%8)));
	}

	//mem copy implementation without overlapping conflicts
	//copy into stack
	blockMeta *oldBlockHead = (blockMeta *)(SUB_BYTES(block,4));
	unsigned int oldSize = oldBlockHead->size -(oldBlockHead->size)%2 - (2*(sizeof(blockMeta)));
	char tempArr[oldSize];
	void *oldDataStart = block;
	char *oldDataStartChar = (char *)(oldDataStart);
	for(int i=0; i< oldSize;i++){
		tempArr[i]=oldDataStartChar[i];
	}

	unsigned int adjFreeSize = oldSize;
	blockMeta *prevTail = (blockMeta *)(SUB_BYTES(oldBlockHead,4));
	blockMeta *nextHead = (blockMeta *)(ADD_BYTES(oldBlockHead,oldBlockHead->size - oldBlockHead->size%2));
	if(prevTail->size %2 ==0){
		adjFreeSize+= prevTail->size;
	}
	if(nextHead->size %2 == 0){
		adjFreeSize+= nextHead->size;
	}
	unsigned int farFree= head->contigFree;
	void *newDataStart;
	#ifdef PRINT_DEBUG
		//printf("adjFree = %u\n", adjFreeSize);
		//printf("farFree = %u\n", farFree);
		//printf("new size = %u\n", new_size);
	#endif
	if((adjFreeSize >= new_size) || (farFree >= new_size)){
		hl_release_no_lock(heap, block);
		//copy back into heap into new location
		newDataStart = hl_alloc_no_lock(heap, new_size);
		if(newDataStart == NULL){
			spin_unlock(malloc_lock);
			return NULL;
		}
		#ifdef PRINT_DEBUG
			//printf("pointer from call to alloc %p\n", newDataStart);
		#endif
		char *newDataStartChar= (char *)(newDataStart);
		for(int j=0; j< oldSize;j++){
			newDataStartChar[j] = tempArr[j];
		}
	}
	else{
		newDataStart = NULL;
	}
	spin_unlock(malloc_lock);
	return newDataStart;
}

