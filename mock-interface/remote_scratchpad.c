#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "remote_scratchpad.h"

void init_remote_memory(size_t block_size, size_t n, useconds_t get_delay,
  useconds_t set_delay) {
  rmem_block_size = block_size;
  rmem_get_delay = get_delay;
  rmem_set_delay = set_delay;

  rmem = malloc(rmem_block_size * n);
}

void destroy_remote_memory() {
  free(rmem);
}

void remote_get(block_id_t src_block_id, void *dst, size_t n) {
  usleep(rmem_get_delay);
  memcpy(dst, rmem+(rmem_block_size*src_block_id), n*rmem_block_size);
}

void remote_set(void *src, block_id_t dst_block_id, size_t n) {
  usleep(rmem_set_delay);
  memcpy(rmem+(rmem_block_size*dst_block_id), src, n*rmem_block_size);
}

void *create_scratchpad(size_t size) {
  return malloc(size);
}

void destroy_scratchpad(void *scratchpad) {
  free(scratchpad);
}
