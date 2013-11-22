#include <stdio.h>

// Define variables based on the configuration of the l1 data cache config
// <name>:<nsets>:<bsize>:<assoc>:<repl>:<pref>
#define NSETS 64
#define BSIZE 64

// struct foo has two arrays of size BSIZE, so accessing a[BSIZE] should
// trigger the nextline prefetcher to put b[BSIZE] in the L1 data cache
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
    // Good for nextline prefetch
    trash_sum += arr[i].a[random_i];
    trash_sum += arr[i].b[random_i];

    // Bad for nextline prefetch
    // The nextline prefetcher won't work well because a[BSIZE] is the previous
    // contiguous block of memory, and our dl1.miss_rate will increase.
    //trash_sum += arr[i].a[random_i];
    //trash_sum += arr[i].b[random_i];
  }
  printf("trash_sum %d\n", trash_sum);
  return 0;
}
