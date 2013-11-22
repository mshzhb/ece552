#include <stdio.h>

// Define variables based on the configuration of the l1 data cache config
// <name>:<nsets>:<bsize>:<assoc>:<repl>:<pref>
#define NSETS 64
#define BSIZE 64

int main(void) {
  int arr_size = 10000 * NSETS;
  char arr[arr_size];
  int trash_sum = 0; // To ensure the compiler doesn't optimize the loop away

  // If increment is BSIZE*2, we will try to access an element that's just over
  // the block that got prefetched by the nextline prefetcher, and our miss
  // rate becomes 53.38%.
  // If increment is BSIZE, we will access an element in the prefetched block,
  // and our miss rate becomes 0.21%.
  int increment = BSIZE;
  int i;
  for (i = 0; i < arr_size; i = i + increment) {
    trash_sum += arr[i];
  }
  printf("trash_sum %d\n", trash_sum);
  return 0;
}
