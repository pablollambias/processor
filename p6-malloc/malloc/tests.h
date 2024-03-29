#include "heaplib.h"
#include "random.h"
#include "assert.h"
#include "printf.h"

typedef unsigned int uintptr_t;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define HEAP_SIZE 1024
#define ARRAY_LEN 16
#define NUM_TESTS 24
#define NPOINTERS 100
#define PRINT_DEBUG

// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
    /* Given SPEC tests */
    /* 0 */ "single init, should return without error",
    /* 1 */ "single init then single alloc, should pass",
    /* 2 */ "single alloc which should fail b/c heap is not big enough",
    /* 3 */ "multiple allocs, verifying no hard-coded block limit",
    /* Malloc Lab  tests */
    /* 4  */ "alloc: block returned is aligned",
    /* 5  */ "alloc: block returned is aligned even when heap is not",
    /* 6  */ "free: acts like nop when block is NULL",
    /* 7  */ "free: allows the block to be used again",
    /* 8  */ "resize: acts like alloc when block is NULL",
    /* Additional Spec tests */
    /* 9  */ "resize: copies values",
    /* 10 */ "EMPTY TEST",
    /* 11 */ "EMPTY TEST",
    /* 12 */ "EMPTY TEST",
    /* 13 */ "EMPTY TEST",
    /* 14 */ "EMPTY TEST",
    /* 15 */ "EMPTY TEST",
    /* Given STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits",
    /* Additional STRESS tests */
    /* 17 */ "EMPTY TEST",
    /* 18 */ "EMPTY TEST",
    /* 19 */ "EMPTY TEST",
    /* 20 */ "EMPTY TEST",
    /* 21 */ "EMPTY TEST",
    /* 22 */ "EMPTY TEST",
    /* 23 */ "EMPTY TEST",
    /* Reserved autograder tests */
    /* 24 */ "EMPTY TEST",
    /* 25 */ "EMPTY TEST",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no?
 */
int test02() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns NULL, test22 returns SUCCESS (==1)
    return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */
int test03() {

    // now we simulate 2 heaps, once 2x size of the first
    char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
    int num_blocks = 0;
    int num_blocks2 = 0;

    hl_init(heap, HEAP_SIZE);

    while(true) {
        int *array = hl_alloc(heap, 8);
        if (array)
            num_blocks++;
        else
            break;
    }

    hl_init(heap2, HEAP_SIZE*2);

    while(true) {
        int *array = hl_alloc(heap2, 8);
        if (array)
            num_blocks2++;
        else
            break;
    }
#ifdef PRINT_DEBUG
    printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
#endif

    // hoping to return SUCCESS (==1)
    return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* FUNCTIONS BEING TESTED: init, alloc
 * SPECIFICATION BEING TESTED:
 * alloc should return ptrs that are 8-byte aligned
 *
 * MANIFESTATION OF ERROR:
 */
int test04() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	char *ourpointer = hl_alloc(heap, 15);
	if(((uintptr_t )ourpointer) % 8 != 0){
    return FAILURE;
  }
  else{
	  return SUCCESS;
}
}

/* FUNCTIONS BEING TESTED: init, alloc
 * SPECIFICATION BEING TESTED:
 * alloc should return ptrs that are 8-byte aligned
 * even when the heap is not 8-byte aligned
 *
 * MANIFESTATION OF ERROR:
 */
int test05() {
	char heap[1025];
	hl_init(heap, 1025);
	char *ourpointer = hl_alloc(heap, 15);
	char *our2 = hl_alloc(heap, 34);
	char *our3 = hl_alloc(heap, 32);
	char *our4 = hl_alloc(heap, 12);
	if(((uintptr_t )ourpointer) % 8 != 0){
    return FAILURE;
  }
  	if(((uintptr_t )our2) % 8 != 0){
    return FAILURE;
  }
  	if(((uintptr_t )our3) % 8 != 0){
    return FAILURE;
  }
  	if(((uintptr_t )our4) % 8 != 0){
    return FAILURE;
  }
  else{
	  return SUCCESS;
}
}

/* FUNCTIONS BEING TESTED: free
 * SPECIFICATION BEING TESTED:  free: acts like nop when block is NULL
 * MANIFESTATION OF ERROR:
 */
int test06() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	char *nullPointer = 0;
	hl_release(heap, nullPointer);

    return SUCCESS;
}


/* FUNCTIONS BEING TESTED: free
 * SPECIFICATION BEING TESTED: freed blocks should be allocatable again
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test07() {
	char heap [2048];
	hl_init(heap, 2048);
	for(int i=0; i<500;i++){
		char *ourPointer = hl_alloc(heap, 2048-(100+(3*i)));
		if(ourPointer == NULL){
			return FAILURE;
		}
		hl_release(heap, ourPointer);
	}

    return SUCCESS;

}

/* FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * hl_resize acts as hl_alloc when it's passed a null blk_ptr
 *
 *
 * MANIFESTATION OF ERROR:
 */
int test08() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	char *nullPointer = 0;
	char *ourPointer = hl_resize(heap, nullPointer, 8);
	if((ourPointer< heap) | (ourPointer > heap +HEAP_SIZE) | (ourPointer >heap +HEAP_SIZE -8)){
		return FAILURE;
	}else{
    return SUCCESS;
}
}
/* FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:alloc and free a bunch of times
 * should copy over the values from its memory area when resizing
 *
 * MANIFESTATION OF ERROR:
 */
int test09() {
	char heap [1024];
	hl_init(heap, 1024);
	char *ourPointer = hl_alloc(heap, 400);
	char *our2 = hl_alloc(heap, 400);
	hl_release(heap, ourPointer);
	if(hl_alloc(heap, 400)==NULL){
		return FAILURE;
	}
    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test10() {
	
	char heap [1024];
	hl_init(heap, 1024);
	
	char *ourPointer = hl_alloc(heap, 200);
	
	char *our2 = hl_alloc(heap, 200);
	//writing to place we allocated
	for (int i = 0; i < 200; i++){
		our2[i] = 'a';
	}
	char *our3= hl_alloc(heap, 200);
	#ifdef PRINT_DEBUG
		printf("before resize \n");
		printf("ourPointer %p\n", ourPointer);
		printf("our2 %p\n", our2);
		printf("our3 %p\n", our3);
	#endif
	char *our4 = hl_resize(heap, our2, 300);
	#ifdef PRINT_DEBUG
		printf("pointer from resize, our4 = %p\n", our4);
	#endif
	for (int i = 0; i < 200; i++){
		if( our4[i] != 'a'){
			return FAILURE;
		}
	}
	
    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc, free, and resize
 * SPECIFICATION BEING TESTED: structural integrity
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test11() {
	
	char heap [2048];
	hl_init(heap, 2048);
	
	for (int i = 0; i < 100; i++){
		#ifdef PRINT_DEBUG
			printf("i= %d\n", i);
		#endif
		char *ourPointer = hl_alloc(heap, 8 + i*3);
		if (ourPointer == NULL){
			return FAILURE;
		}
		char *our2 = hl_alloc(heap, 1000 - i*3);
		if (our2 == NULL){
			return FAILURE;
		}
		char *our3 = hl_resize(heap, ourPointer, (8 + i*3) + i);
		if (our3 == NULL){
			return FAILURE;
		}
		hl_release(heap, our2);
		hl_release(heap, our3);
	}
	
	
		
    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test12() {

    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test13() {

    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test14() {

    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test15() {

    return FAILURE;
}

/* ------------------ STRESS TESTS ------------------------- */

/* FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {
    int n_tries    = 10000;
    int block_size = 16;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

	char *before = memarea;
	char *after = &memarea[2048];

	// zero out the buffers before and after
	memset(before, 0, NPOINTERS*sizeof(char *));
	memset(after, 0, NPOINTERS*sizeof(char *));

	for (int i = 0; i < NPOINTERS; i++) {
		before[i] = 'b';
		after[i] = 'a';
	}

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
        }
        else{
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }

	for (int i = 0; i < NPOINTERS; i++) {
		assert(before[i] == 'b');
		assert(after[i] == 'a');
	}

    return SUCCESS;
}

struct info {
    void *ptr;
    int size;
    char seed;
} info[NPOINTERS];

struct info3 {
    void *ptr;
    int size;
    char seed;
} info3[NPOINTERS];

/* Fill chunk of memory with content.
*/
static inline void fill(char *p, int size, char initial) {
    int i;
    for (i = 0; i < size; ++i) {
        *p++ = initial++;
    }
}

/* Verify the data in a memory with the content
 */
static inline int verify(char *p, int size, char initial) {
    int i;
    for (i = 0; i < size; ++i) {
        if (*p++ != initial++) {
            return false;
        }
    }
    return true;
}

/* FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:alloc & free integrity test,
 * MANIFESTATION OF ERROR:
 *
 */
int test17() {


  char heap[HEAP_SIZE];

  int x = 0;

  hl_init(heap, HEAP_SIZE);

  char* ptr_array[2000];

  for(int i = 0; i<2000; i++){
    int k = random()%1024;
    //printf("Random is %d\n", k);
    ptr_array[i] = hl_alloc(heap, k);
  }


int k = 0;
  while(k <2000){
    hl_resize(heap, ptr_array[k], random()%1024);
    k = k + random()%1024;
  }


  int j = 0;
  while(j <2000){
    hl_release(heap, ptr_array[j]);
    j = j + random()%1024;
  }

    return SUCCESS;
}

/* FUNCTIONS BEING TESTED: alloc, free, resize
 * SPECIFICATION BEING TESTED:alloc, free, & resize integrity test
 * The returned address should be aligned at multiples of 8 bytes
 * MANIFESTATION OF ERROR:
 *
 */
int test18() {
    int n_tries    = 10000;
    int block_size = 16;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        block_size = random() % 100;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if ((unsigned int)(pointers[index]) % 8 != 0)
                return FAILURE;
        }
        else{
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }
    return SUCCESS;
}

/* FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:alloc & free data integrity test,
* Data Integrity of Allocate. Once data is allocated, it should not be accidentally overridden
 * MANIFESTATION OF ERROR:
 * If we randomize block size, and store different chars in each allocation, we can maintain 
 * a list of pointers, a list of sizes, and a list of chars that indicate what each allocation
 * contains and how big it is. When we come across that pointer again in the forloop, 
 * we check to make sure that the data is the same as when it was allocated
 *
 */
int test19() {
    int n_tries    = 10000;
    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    // Size array - to hold the size of each block
    int stored_size[NPOINTERS];
    // Holds the value that is in the particular block
    char stored_char[NPOINTERS];


    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {

            int random_block_size = random()% 100;
            pointers[index] = hl_alloc(heap_start, random_block_size);
            int k;
            if (pointers[index] != NULL){
                // randomize character
                char randomletter = 'a' + (random() % 26);
                stored_size[index] = random_block_size;
                stored_char[index] = randomletter;
                for (k =0; k < random_block_size; k++){
                    pointers[index][k] = randomletter;

                }
            }
            else{
                stored_size[index] = 0;
                stored_char[index] = 0;
            }
        }
    
        else{
            int j;
            for(j =0; j < stored_size[index]; j++){
                if (pointers[index][j] != stored_char[index])
                    return FAILURE;
            }
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
            stored_size[index] = 0;
        }
    }
    return SUCCESS;
}


int test20() {
    
    return SUCCESS;
}
 FUNCTIONS BEING TESTED: alloc, free, resize
 * SPECIFICATION BEING TESTED:alloc, free, resize - a lot of times
* Data Integrity of Resize. Ensures that resize preserves the contents of the original allocation
 * MANIFESTATION OF ERROR:
 * If we randomize block size and store different chars in each allocation, 
 * when we reallocate the pointer to a random smaller or larger size, 
 * the specific char's inside the original allocation must be maintained
 *
 
int test21() {
    int n_tries = 10000;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS]; //creates an array of 100 (NPointers) pointers
    //this pointer array is not in the heap

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    //sets the seed for the random function
    srandom(0);
    //for 100000 iterations - loop.
    for (int i = 0; i < n_tries; i++) {
        //randomly sets the index - index of the pointer array - that is we are selecting a random pointer
        //by modding the random number by NPOINTERS we end up with a value that is in fact bewteen 0 and NPOINTERS
        int index = random() % NPOINTERS;
        //if the element at that index == 0 - that is it doesn't point anywhere
        if (pointers[index] == 0) {

            //other random intitializations
            int random_block = random()% 100 +8;   //generates a block size 8 to 107
            int block_scalar = random()% 30 -8;      //generates a block_scalar -8 to 24


            //Since we have confirmed that the index is empty, we now fill it up with a pointer to a newly allocated block
            pointers[index] = hl_alloc(heap_start, random_block);
            //initilize k - for the loop
            int k;
            char randomletter = 'a' + (random() % 26);

            //if the allocation was valid/ alloc didn't return null we can now populatate that block with randomletter
            if (pointers[index] != NULL){
                for (k =0; k < random_block; k++){
                    pointers[index][k] = randomletter;
                }
            

                unsigned int newBlock = random_block + block_scalar;                      // fuck up
                pointers[index] = hl_resize(heap_start, pointers[index], newBlock);
                
                //if the pointer to the new block is not NUll --> compare values inside
                if((newBlock > random_block) && (pointers[index]!= NULL)){
                    //checks to make sure the old contents are in the new block
                    for (k =0; k < random_block; k++){
                        if(pointers[index][k] != randomletter){
                            return FAILURE;
                        }
                    }
                    //populates the new part of the block
                    for(k = random_block; k < newBlock; k++){
                        pointers[index][k] = randomletter;
                    }
                }
                else if (pointers[index] != NULL){
                //check to make sure the block retains all the old values of the old block size 
                    for (k =0; k < newBlock; k++){
                        if(pointers[index][k] != randomletter)
                            return FAILURE;
                    }

                }
            }
        }
    
        else{
            //if the randomly indexed spot already has a pointer and our random var says we should release, we now free it
            hl_release(heap_start, pointers[index]);
            //to denote that we have freed it, pointer[index] = 0 
            pointers[index] = 0;
            
        }
    }
    return SUCCESS;
}

/* FUNCTIONS BEING TESTED: lock and unlock
 * SPECIFICATION BEING TESTED: set lock to 1 and try grabbing it. should result in infinate loop
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {
    return FAILURE;
}

/* FUNCTIONS BEING TESTED: 
 * SPECIFICATION BEING TESTED:
 * MANIFESTATION OF ERROR:
 *
 */
int test23() {

    return FAILURE;
}

/* RESERVED FOR AUTOGRADER */

int test24() {

    return FAILURE;
}
int test25() {

    return FAILURE;
}
