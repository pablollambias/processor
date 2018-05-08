
#ifndef MEM_H_
#define MEM_H_
#include "concurrency.h"
/* mem.c */

#define NOPAGE 0xFFFFFFFF // physical address go from 0 to at most 1 GB, so 0xFFFFFFFF can serve as a "null" physical address

void mem_init();

// note: these are not synchronized: if someone is mucking with the pagetables
// while these are running, something could go wrong.
inline static void *physical_to_virtual(unsigned int paddr); // return some usable virtual address (not necessarily unique) for given physical address
inline static unsigned int virtual_to_physical_fast(void *vptr);
unsigned int virtual_to_physical(void *vptr); // return physical address that given virtual address maps to, or NOPAGE if there is no mapping

// standard library helpers
void *memset(void *s, unsigned int c, unsigned int len);
void *memcpy(void *dest, const void *src, unsigned int len);

// fast, underlying page-at-a-time memory management
// note: These are not synchronized in any way. If multiple cores are going to
// call these functions, then every access to these need to be use some
// multi-core safe synchronization. And if interrupt handlers are going to call
// these functions, then every access to these needs to use interrupt-safe
// synchronization too.
void *alloc_pages(unsigned int count); // allocate count pages of physically (and virtually) contiguous memory
void *calloc_pages(unsigned int count); // allocate and clear count pages of physically (and virtually) contiguous memory
void free_pages(void *page, unsigned int count); // free count contiguous pages that came from page_alloc() and/or page_calloc()

// standard memory management
// note: These are not synchronized in any way. If multiple cores are going to
// call these functions, then every access to these need to be use some
// multi-core safe synchronization. And if interrupt handlers are going to call
// these functions, then every access to these needs to use interrupt-safe
// synchronization too.
void *malloc(unsigned int size); // allocate size bytes of memory
void *calloc(unsigned int size, unsigned int count); // allocate and clear (size * count) bytes of memory
void free(void *p); // free a pointer that came from malloc() or calloc()

#endif
