#include <stdio.h>

// <name>:<nsets>:<bsize>:<assoc>:<repl>:<pref>
#define NSETS 64
#define BSIZE 64
struct foo {
  char a[BSIZE];
  char b[BSIZE];
};


int main(void) {
  struct foo arr[NSETS];
  int trash_sum = 0;
  int random_i = 23;
  int i;
  for (i = 0; i < NSETS; i++) {
    // Good for nextline prefetch
    trash_sum += arr[i].a[random_i];
    trash_sum += arr[i].b[random_i];

    // Bad for nextline prefetch
    //trash_sum += arr[i].a[random_i];
    //trash_sum += arr[i].b[random_i];
  }
  printf("trash_sum %d\n", trash_sum);
  return 0;
}
