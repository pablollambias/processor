#include "heaplib.h"
#include "stdlib.h"
#include "stdio.h"
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr))+(num_bytes))


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
	// preconditions: heap_size>1024, the allocated heap must be heap_size in bytes
	// have a header which acts like "global variables" with heap size, open bytes left and num blocks
	// block attributes: blockSize, inUse (bool)
	int *heapBy4 = (int *)heap;
	//header (global var) contains size of biggest block
	heapBy4[0] = heap_size - (sizeof(int)) - ((heap_size - sizeof(int))%8); 
	//initializing the rest of the heap to be one unused block (with a header and footer)
	heapBy4[sizeof(int)] = heap[0];
	//[ADD_BYTES(heap,sizeof(int)] = heap[0];
	heapBy4[heapBy4[0]-sizeof(int)] = heap[0];
	//[ADD_BYTES(heap, heap[0]-sizeof(int)) = heap[0];
	
	
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
	//preconditions. hl_init must have been called exactly once for this heap
	//find the "best" place in the heap to put this block, and update the block info at the beginning of the heap
    return NULL;
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
	// find the block that we would like to free and change the inUse attribute to False;
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
	//find the block that we wish to resize, check if there is contiguous space
	// if there is change the block size and we are set
	// if there is not space need to find new space big enough in heap, copy over the old values and free old block
    return NULL;
}
