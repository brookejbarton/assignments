#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"
#define ROUNDS 10
#define LOOP 100000

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  printf("The initial top of the heap is %p.\n", init);

  void *current = sbrk(0);
  printf("The current top of the heap is %p.\n", current);

  int allocated = (int) ((current - init));
  printf("Increased by %d (0x%x) bytes\n", allocated, allocated);

  return 0 ;
}
