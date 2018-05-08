#ifndef CONCURRENCY_H_
#define CONCURRENCY_H_

/* Mutex
 *
 * A mutex lock implemented in assembly.
 */

// The type of the mutex.
typedef volatile unsigned int mutex;

// Lock the mutex.
void mutex_lock(mutex* m);
// Unlock the mutex.
void mutex_unlock(mutex* m);

#endif
