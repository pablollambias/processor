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

    return FAILURE;
}

/* FUNCTIONS BEING TESTED: init, alloc
 * SPECIFICATION BEING TESTED:
 * alloc should return ptrs that are 8-byte aligned
 * even when the heap is not 8-byte aligned
 *
 * MANIFESTATION OF ERROR:
 */
int test05() {

    return FAILURE;
}

/* FUNCTIONS BEING TESTED: free
 * SPECIFICATION BEING TESTED:
 * hl_release of zero acts as a nop
 *
 * MANIFESTATION OF ERROR:
 */
int test06() {

    return FAILURE;
}

#define NPOINTERS3 5

/* FUNCTIONS BEING TESTED: free
 * SPECIFICATION BEING TESTED: freed blocks should be allocatable again
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test07() {

    return FAILURE;

}

/* FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * hl_resize acts as hl_alloc when it's passed a null blk_ptr
 *
 *
 * MANIFESTATION OF ERROR:
 */
int test08() {

    return FAILURE;

}

/* FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * should copy over the values from its memory area when resizing
 *
 * MANIFESTATION OF ERROR:
 */
int test09() {

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
int test10() {

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
int test11() {

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
    return FAILURE;
}

/* FUNCTIONS BEING TESTED: alloc, free, resize
 * SPECIFICATION BEING TESTED:alloc, free, & resize integrity test
 * MANIFESTATION OF ERROR:
 *
 */
int test18() {
    
    return FAILURE;
}

/* FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:alloc & free data integrity test,
 * MANIFESTATION OF ERROR:
 *
 */
int test19() {
    
    return FAILURE;
}

int test20() {
    
    return FAILURE;
}
/* FUNCTIONS BEING TESTED: alloc, free, resize
 * SPECIFICATION BEING TESTED:alloc, free, resize - a lot of times
 * MANIFESTATION OF ERROR:
 *
 */
int test21() {
    
    return FAILURE;
}

/* FUNCTIONS BEING TESTED: 
 * SPECIFICATION BEING TESTED:
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
