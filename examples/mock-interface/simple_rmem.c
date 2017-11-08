#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "remote_scratchpad.h"

int main(int argc, char **argv) {
  init_remote_memory(4096, 64, 2, 1);

  // Fill remote memory with 42s.
  size_t bsize = (4096*64) / sizeof(int);
  int *buffer = (int *) malloc(sizeof(int) * bsize);

  for (int i = 0; i < bsize; i++) {
    buffer[i] = 42;
  }

  remote_set(buffer, 0, 64);

  int *buffer2 = (int *) malloc(sizeof(int) * bsize);

  remote_get(0, buffer2, 64);

  int cmp = memcmp(buffer2, buffer, sizeof(int) * bsize);

  if (cmp == 0) {
    printf("Success!! Memory blade stored zeros.\n");
  } else {
    printf("Failure!! The memory blade misbehaved.\n");
  }

  free(buffer);
  free(buffer2);

  destroy_remote_memory();
  return 0;
}
