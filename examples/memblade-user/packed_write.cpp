#include <cstdio>
#include <cstdlib>

extern "C" {
  #include <remote_scratchpad.h>
}

#include "PackedWrite.hpp"
#include "CachedRead.hpp"

int main(int argc, char **argv) {
  init_remote_memory(4096, 1000, 1, 1);
  PackedWrite write(4096, 1000);

  int n = 2;

  write.write(0, sizeof(int), &n);
  write.write(16, sizeof(int), &n);
  write.write(1034, sizeof(int), &n);
  char *x = (char *) malloc(5000);
  for (int i = 0; i < 5000; i++) {
    x[i] = 'a' + (i % 10);
  }
  write.write(4000, 5000, x);

  write.print();

  printf("Issuing...\n");
  write.issue();
  printf("Issued!\n");
  write.print();

  CachedRead read(4096, 1000);

  char *y = (char *) malloc(5000);
  read.read(4000, 5000, y);
  if (memcmp(x, y, 5000) != 0) {
    printf("Error! Send or read did not work.\n");
  } else {
    printf("Everything seems to have gone smoothly.\n");
  }
  free(y);
  free(x);

  destroy_remote_memory();
  return 0;
}
