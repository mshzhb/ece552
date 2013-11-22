#include <stdio.h>

// Define variables based on the configuration of the l1 data cache config
// <name>:<nsets>:<bsize>:<assoc>:<repl>:<pref>
#define NSETS 64
#define BSIZE 64

int main(void) {
  int arr_size = 10000 * NSETS;
  char arr[arr_size];
  int trash_sum = 0; // To ensure the compiler doesn't optimize the loop away

  int i;
  for (i = 0; i < arr_size; i++) {
    trash_sum += arr[i];

    // If we offset i by a different stride each time, the stride prefetcher
    // won't do a good job predicting which block to fetch, and our miss rate
    // becomes 60.21%.
    // If we offset i by a constant stride, the stride prefetcher does well
    // and our miss rate becomes 0.45%.
    i = i % 2 ? i + BSIZE : i + 2*BSIZE;
    //i = i + 2*BSIZE;
  }
  printf("trash_sum %d\n", trash_sum);
  return 0;
}
