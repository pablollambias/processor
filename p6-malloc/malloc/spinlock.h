#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

typedef volatile unsigned int slock;

extern slock* malloc_lock;

void spin_lock(slock* l);
void spin_unlock(slock* l);

#endif
