#include <cstdio>
#include <cstdlib>

extern "C" {
  #include <remote_scratchpad.h>
}

#include "PackedWrite.hpp"

int main(int argc, char **argv) {
  init_remote_memory(4096, 1000, 1, 1);
  PackedWrite write(1000, 4096);

  int n = 2;

  write.write(0, sizeof(int), &n);
  write.write(16, sizeof(int), &n);
  write.write(1034, sizeof(int), &n);
  char *x = (char *) malloc(5000);
  for (int i = 0; i < 5000; i++) {
    x[i] = 'a' + (i % 10);
  }
  write.write(4000, 5000, x);
  free(x);

  write.print();

  printf("Issuing...\n");
  write.issue();
  printf("Issued!\n");
  write.print();

  destroy_remote_memory();
  return 0;
}
