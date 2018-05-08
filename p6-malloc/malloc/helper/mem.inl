#ifndef MEM_INL_
#define MEM_INL_

inline void *physical_to_virtual(unsigned int paddr)
{
  // By default, virtual address 0xC0000000 + N is mapped to physical address N.
  // There may other virtual addresses that also map to physical address N, but
  // for this function any one will do, so we use the 0xC0000000 + N mapping.
  return (void *)(0xC0000000 + paddr);
}

inline unsigned int virtual_to_physical_fast(void *vptr)
{
  return (vptr - (void *)0xC0000000);
}

#endif
