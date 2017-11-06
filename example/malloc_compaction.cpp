#include <cstdlib>
#include <cstdio>
#include <vector>

extern "C" {
  #include <remote_scratchpad.h>
  void print_mem_list(void *scratchpad);
}

int main(int argc, char **argv) {
  size_t n = 65536;
  void *scratchpad = create_scratchpad(n);
  init_scratchpad_malloc(scratchpad);

  int *a = (int *) scratch_malloc(scratchpad, 128);
  int *b = (int *) scratch_malloc(scratchpad, 128);
  int *c = (int *) scratch_malloc(scratchpad, 128);

  print_mem_list(scratchpad);
  scratch_free(scratchpad, a);
  print_mem_list(scratchpad);
  scratch_free(scratchpad, b);
  print_mem_list(scratchpad);

  int *d = (int *) scratch_malloc(scratchpad, 256);
  print_mem_list(scratchpad);


  int *e = (int *) scratch_malloc(scratchpad, 128);
  int *f = (int *) scratch_malloc(scratchpad, 128);
  int *g = (int *) scratch_malloc(scratchpad, 128);

  print_mem_list(scratchpad);
  scratch_free(scratchpad, f);
  print_mem_list(scratchpad);
  scratch_free(scratchpad, e);
  print_mem_list(scratchpad);

  destroy_scratchpad(scratchpad, n);

  return 0;
}
