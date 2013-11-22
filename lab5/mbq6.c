#include <stdio.h>

// Define variables based on the configuration of the l1 data cache config
// <name>:<nsets>:<bsize>:<assoc>:<repl>:<pref>
#define NSETS 64
#define BSIZE 64

// The open prefetcher is a hybrid of the stride prefetcher and a data address
// miss queue. Each time a data access is missed in the cache, it gets enqueued
// in the data address miss queue.
// The open prefetcher first checks whether or not we can do a stride prefetch.
// We only do a stride prefetch if the entry is in the steady state.
// Otherwise, we check the data address miss queue and look for the address
// that we missed. If we find it, we return the next address in the queue and
// hope that that's the next data address being accessed.
int main(void) {
  // Depending on how big arr_size is, the data that we access might not
  // fit entirely into the data address miss queue used by our open predictor.
  // If the size is 1000 * NSETS, then all of the data that we access will fit
  // into our miss queue ((1000 * NSETS) / BSIZE = 1000 < 2048), and our miss
  // rate will be 0.01%!
  // If the size is 10000 * NSETS, then the data that we access won't fit into
  // our miss queue ((10000 * NSETS) / BSIZE = 10000 > 2048), and our miss rate
  // will be 100%.
  int arr_size = 1000 * NSETS;
  char arr[arr_size];
  int trash_sum = 0; // To ensure the compiler doesn't optimize the loop away

  // We have an outer loop here so that the open prefetcher is able to take
  // advantage of the data address miss queue
  int i, j;
  for (j = 0; j < 100000; j++) {
    for (i = 0; i < arr_size; i++) {
      trash_sum += arr[i];

      // If we offset i by a different stride each time, the stride prefetcher
      // won't do a good job predicting which block to fetch. We will fall back
      // to looking into the data address miss queue in this case.
      i = i % 2 ? i + BSIZE : i + 2*BSIZE;
    }
  }
  printf("trash_sum %d\n", trash_sum);
  return 0;
}
