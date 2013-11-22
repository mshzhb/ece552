#include <stdio.h>

// Define variables based on the configuration of the l1 data cache config
// <name>:<nsets>:<bsize>:<assoc>:<repl>:<pref>
#define NSETS 64
#define BSIZE 64

// struct foo has two arrays of size BSIZE. The miss rate should be more
// invariant to whether or not you access a[BSIZE] or b[BSIZE] first because
// strides can be negative.
struct foo {
  char a[BSIZE];
  char b[BSIZE];
};

int main(void) {
  // Allocate NSETS of struct foo to fill up the L1 data cache. This is twice
  // as much data the L1 data cache can hold, which ensures that there will be
  // some misses and prefetches being done.
  struct foo arr[NSETS];
  int trash_sum = 0; // To ensure the compiler doesn't optimize the loop away
  int random_i = 23; // Some random element so the cache will respond
  int i;
  for (i = 0; i < NSETS; i++) {
    // Access a[BSIZE] then b[BSIZE]
    //trash_sum += arr[i].a[random_i];
    //trash_sum += arr[i].b[random_i];

    // Access b[BSIZE] then a[BSIZE]. The dl1.miss_rate should be invariant
    // because strides can be positive or negative. In this case, the stride
    // is negative, so the prefetcher will put a[BSIZE] in the L1 data cache.
    trash_sum += arr[i].b[random_i];
    trash_sum += arr[i].a[random_i];
  }
  printf("trash_sum %d\n", trash_sum);
  return 0;
}
