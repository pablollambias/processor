//-*- mode:c -*-
#ifndef HASHTABLE_INL_
#define HASHTABLE_INL_

inline hash compute_hash(const unsigned char* c, unsigned int len) {
#ifdef HASH_USE_LOOKUP
  hash hash = 5381;
  unsigned int i = 0;

  //get make sure c[i] is aligned on a half word boundry
  while (((unsigned int)(c + i) & 1) != 0)
    hash = hash * 33 + c[i++];

  while (i + 63 < len) {
    hash = hash * 2463752705 + 
      hash_w3_h[*((unsigned short*)(c+i))] +
      hash_w3_l[*((unsigned short*)(c+i+2))] +
      hash_w2_h[*((unsigned short*)(c+i+4))] +
      hash_w2_l[*((unsigned short*)(c+i+6))] +
      hash_w1_h[*((unsigned short*)(c+i+8))] +
      hash_w1_l[*((unsigned short*)(c+i+10))] +
      hash_w0_h[*((unsigned short*)(c+i+12))] +
      hash_w0_l[*((unsigned short*)(c+i+14))];
    hash = hash * 2463752705 + 
      hash_w3_h[*((unsigned short*)(c+i+16))] +
      hash_w3_l[*((unsigned short*)(c+i+18))] +
      hash_w2_h[*((unsigned short*)(c+i+20))] +
      hash_w2_l[*((unsigned short*)(c+i+22))] +
      hash_w1_h[*((unsigned short*)(c+i+24))] +
      hash_w1_l[*((unsigned short*)(c+i+26))] +
      hash_w0_h[*((unsigned short*)(c+i+28))] +
      hash_w0_l[*((unsigned short*)(c+i+30))];
    hash = hash * 2463752705 + 
      hash_w3_h[*((unsigned short*)(c+i+32))] +
      hash_w3_l[*((unsigned short*)(c+i+34))] +
      hash_w2_h[*((unsigned short*)(c+i+36))] +
      hash_w2_l[*((unsigned short*)(c+i+38))] +
      hash_w1_h[*((unsigned short*)(c+i+40))] +
      hash_w1_l[*((unsigned short*)(c+i+42))] +
      hash_w0_h[*((unsigned short*)(c+i+44))] +
      hash_w0_l[*((unsigned short*)(c+i+46))];
    hash = hash * 2463752705 + 
      hash_w3_h[*((unsigned short*)(c+i+48))] +
      hash_w3_l[*((unsigned short*)(c+i+50))] +
      hash_w2_h[*((unsigned short*)(c+i+52))] +
      hash_w2_l[*((unsigned short*)(c+i+54))] +
      hash_w1_h[*((unsigned short*)(c+i+56))] +
      hash_w1_l[*((unsigned short*)(c+i+58))] +
      hash_w0_h[*((unsigned short*)(c+i+60))] +
      hash_w0_l[*((unsigned short*)(c+i+62))];
    i += 64;
  }

  while (i + 15 < len) {
    hash = hash * 2463752705 + 
      hash_w3_h[*((unsigned short*)(c+i))] +
      hash_w3_l[*((unsigned short*)(c+i+2))] +
      hash_w2_h[*((unsigned short*)(c+i+4))] +
      hash_w2_l[*((unsigned short*)(c+i+6))] +
      hash_w1_h[*((unsigned short*)(c+i+8))] +
      hash_w1_l[*((unsigned short*)(c+i+10))] +
      hash_w0_h[*((unsigned short*)(c+i+12))] +
      hash_w0_l[*((unsigned short*)(c+i+14))];
    i += 16;
  }

  while (i + 7 < len) {
    hash = hash * 1954312449 + 
      hash_w1_h[*((unsigned short*)(c+i))] +
      hash_w1_l[*((unsigned short*)(c+i+2))] +
      hash_w0_h[*((unsigned short*)(c+i+4))] +
      hash_w0_l[*((unsigned short*)(c+i+6))];
    i += 8;
  }
  while (i < len)
    hash = hash * 33 + c[i++];

  return hash;
#else
  hash hash = 5381;
  int i = 0;
  while (i + 32 < len) {
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
  }
  while (i + 8 < len) {
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
    hash = hash * 33 + c[i++];
  }
  while (i < len)
    hash = hash * 33 + c[i++];

  return hash;
#endif
}

inline void hashtable_add(struct hashtable* table, hash key) {
  unsigned int index = key % HASHTABLE_SIZE;
#ifdef HASHTABLE_SAFETY_CHECK
  unsigned int sIndex = index;
#endif

  //  if (current_cpu_id() == 21) {
  //    printf("adding key: %x\n", key);
  //  }

  while (table->statuses[index] == HASHTABLE_OCCUPIED) {
    index = (index + 1) % HASHTABLE_SIZE;
#ifdef HASHTABLE_SAFETY_CHECK    
    if (index == sIndex) {
      puts ("attempted to add into full hashtable");
      return;
    }
#endif
  }

  table->statuses[index] = HASHTABLE_OCCUPIED;
  table->keys[index] = key;
  table->values[index] = 0;
}

inline void hashtable_remove(struct hashtable* table, hash key) {
  unsigned int index = key % HASHTABLE_SIZE;
#ifdef HASHTABLE_SAFETY_CHECK
  unsigned int sIndex = index;
#endif

  while (table->keys[index] != key) {
    index = (index + 1) % HASHTABLE_SIZE;
#ifdef HASHTABLE_SAFETY_CHECK
    if (/*table->statuses[index] == HASHTABLE_EMPTY ||*/ index == sIndex) {
      if(current_cpu_id() == 21)
	printf("attempted to remove element not in hashtable: %x (core id = %d)\n", key, current_cpu_id());
      return;
    }
#endif
  }

  //  if (current_cpu_id() == 21) {
  //    printf("removing key: %x\n", key);
  //  }    
  table->statuses[index] = HASHTABLE_DELETED;
}

inline void hashtable_increment(struct hashtable* table, hash key) {
  unsigned int index = key % HASHTABLE_SIZE;
  unsigned int sIndex = index;
  
  while (table->keys[index] != key &&
    table->statuses[index] != HASHTABLE_EMPTY) {
    index = (index + 1) % HASHTABLE_SIZE;
    
    if (index == sIndex) {
      puts("hashtable is full... ");
      return;
    }
  }

  if (table->statuses[index] != HASHTABLE_EMPTY) {
    table->values[index]++;
  }
}

inline void hashtable_clear(struct hashtable* table) {
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    table->statuses[i] = HASHTABLE_EMPTY;
    table->values[i] = 0;
  }
}


inline void combinetable_combine(struct combinetable* table,
				 struct hashtable* other) {
  /* This function assumes that the two hashtables are identically
     laid out in memory.  This is a fair assumption the only time we
     use this function. */
  mutex_lock(&table->lock);
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    table->values[i] += other->values[i];
  }
  mutex_unlock(&table->lock);
}

inline void combinetable_clear(struct combinetable* table) {
  mutex_lock(&table->lock);
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    table->values[i] = 0;
  }
  mutex_unlock(&table->lock);
}

#endif
