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

  std::vector <int *> arrs;

  for (int i = 0; i < 16; i++) {
    arrs.push_back((int *) scratch_malloc(scratchpad, sizeof(int) * 128));
  }

  print_mem_list(scratchpad);


  for (int i = 0; i < arrs.size(); i++) {
    for (int j = 0; j < 128; j++) {
      arrs[i][j] = i;
    }
  }

  bool allgood = true;

  std::vector <int> free_arrs = {1, 7, 2, 4};


  for (int i = 0; i < free_arrs.size(); i++) {
    scratch_free(scratchpad, arrs[i]);
    print_mem_list(scratchpad);
    arrs.erase(arrs.begin() + i);
  }

  for (int i = 0; i < 2; i++) {
    print_mem_list(scratchpad);
    arrs.push_back((int *) scratch_malloc(scratchpad, sizeof(int) * 128));
  }

  for (int i = 0; i < arrs.size(); i++) {
    for (int j = 0; j < 128; j++) {
      arrs[i][j] = i;
    }
  }

  for (int i = 0; i < arrs.size(); i++) {
    for (int j = 0; j < 128; j++) {
      if (arrs[i][j] != i) {
        fprintf(stderr, "Uh oh, something went wrong in scratch_malloc...\n");
        fprintf(stderr, "Array %d, index %d %d != %d\n", i, j, arrs[i][j], i);
        allgood = false;
        break;
      }
    }
  }

  if (allgood) {
    printf("W00t! Seems like everything went well.\n");
  } else {
    printf("Malloc did not behave as expected.\n");
  }

  destroy_scratchpad(scratchpad);
  return 0;
}
