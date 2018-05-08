#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "concurrency.h"

/* Hashing function
 *
 * We have two implementations of our hashing function: one that only unrolls
 * the look, and one that uses a pre-computed lookup table to speed up the
 * hashing even further.
 */

// When defined, use the lookup table implementation.
#define HASH_USE_LOOKUP

// A hashed value.
typedef unsigned int hash;

// Hash lookup tables.
extern hash hash_w0_l[65536];
extern hash hash_w0_h[65536];
extern hash hash_w1_l[65536];
extern hash hash_w1_h[65536];
extern hash hash_w2_l[65536];
extern hash hash_w2_h[65536];
extern hash hash_w3_l[65536];
extern hash hash_w3_h[65536];

// Hash a sequence of bytes of some length.
inline static hash compute_hash(const unsigned char* c, unsigned int len);

/* Hashtable
 *
 * struct hashtable is a non-threadsafe hashtable indexed by a hash and holding
 * an integer value.  The hash must be provided by the user of the hashtable.
 * The only operations allowed on the values stored in the hashtable are to
 * increment the value stored in the hashtable and to reset the value back to
 * zero.  The only time a value can be read is after it is stored into a combine
 * table.
 *
 * In order to allow the hashtable to be statically allocated, we deal with hash
 * collisions using linear probing.  This makes the worst case for any operation
 * O(n).
 *
 * The result of adding an entry to the hashtable that is full is undefined.
 *
 * The hashtable entries have three possible states: they may be empty, which
 * allows a new entries to be added in the entry; occupied, which allows the
 * value in the entry to be incremented; or deleted, which allows a new element
 * to be added in the entry, but which keeps the value.  This is required to
 * print deleted entries' counts.  Empty entries always have a value of zero.
 *
 * This hashtable is not threadsafe.  It should only be used by one thread.
 *
 * The hashtable provides the following functions:
 *   hashtable_add() adds a hash to the hashtable and sets its value to zero
 *   hashtable_remove() prevents the value from being incremented
 *   hashtable_increment() increments an occupied entry's value
 *   hashtable_clear() resets all entries to empty.
 */

// Number of slots in the hashtable.
#define HASHTABLE_SIZE 768

// Give an error when the hashtable is full instead of silently failing.
//#define HASHTABLE_SAFETY_CHECK

// The hashtable structure.  Entries are separated into three arrays.
struct hashtable {
  hash keys[HASHTABLE_SIZE];
  unsigned int values[HASHTABLE_SIZE];
  enum {
    HASHTABLE_EMPTY    = 0,
    HASHTABLE_OCCUPIED = 1,
    HASHTABLE_DELETED  = -1
  } statuses[HASHTABLE_SIZE];
};

// Add a hash to the hashtable.
inline static void hashtable_add(struct hashtable* table, hash key);
// Remove a hash from the hashtable.
inline static void hashtable_remove(struct hashtable* table, hash key);
// Increment the value stored at the hash in the hashtable.
inline static void hashtable_increment(struct hashtable* table, hash key);
// Clear all entries from the hashtable.
inline static void hashtable_clear(struct hashtable* table);

/* Combine table
 *
 * struct combinetable is a threadsafe table with the same structure as a
 * hashtable holding an integer values.  This is used for combining together
 * many hashtables for printing the results.
 *
 * The results of combining hashtables are only defined when they have the same
 * hashes in them.
 *
 * The combinetable provides the following functions:
 *   combinetable_combine() combines a hashtable with a combinetable.
 *   combinetable_clear() clears a hashtable by setting values to zero.
 */
struct combinetable {
  unsigned int values[HASHTABLE_SIZE];
  mutex lock;
};

// Combines a hashtable with a combine table.
inline static void combinetable_combine(struct combinetable* table,
					struct hashtable* other);
// Clears a hashtable.
inline static void combinetable_clear(struct combinetable* table);

#endif
