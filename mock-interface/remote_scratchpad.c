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

// lil helper stuff for scratch_malloc
#define FREE_BIT 0

#define SMALLEST_MEM_UNIT 64

typedef struct chunk_t {
  size_t size;
  struct chunk_t *next;
  int metadata;
} chunk_t;

int enable_bit(int *metadata, int bit) {
  *metadata = *metadata | (0x1 << bit);
  return *metadata;
}

int disable_bit(int *metadata, int bit) {
  *metadata = *metadata & ~(0x1 << bit);
  return *metadata;
}

int get_bit(int metadata, int bit) {
  return !!(metadata & (0x1 << bit));
}

void *init_scratchpad_malloc(void *scratchpad) {
  chunk_t *chunk = (chunk_t *) scratchpad;

  chunk->size = 0;
  chunk->next = NULL;
  // I do declare this chunk is free.
  chunk->metadata = 0x0;
  enable_bit(&chunk->metadata, FREE_BIT);
  return scratchpad;
}

#include <stdio.h>

void *scratch_malloc(void *scratchpad, size_t size) {
  chunk_t *chunk = (chunk_t *) scratchpad;

  size = (size > SMALLEST_MEM_UNIT) ? size : SMALLEST_MEM_UNIT;

  // While you're not at the end of the list (chunk->next == NULL)
  // and the current chunk is either not free or too small, keep
  // iterating through list.
  while (chunk->next != NULL && (!get_bit(chunk->metadata, FREE_BIT) || chunk->size < size)) {
    // If current chunk is free and followed by another free chunk, compact.
    if (get_bit(chunk->metadata, FREE_BIT) && get_bit(chunk->next->metadata, FREE_BIT)) {
      chunk_t *next_chunk = chunk->next;
      chunk->next = next_chunk->next;
      // next_chunk's data portion (size bytes) and metadata portion
      // (sizeof(chunk_t) bytes) are now gobbled by chunk's data portion
      chunk->size += next_chunk->size + sizeof(chunk_t);
    } else {
      chunk = chunk->next;
    }
  }

  if (chunk->next == NULL || chunk->size - size >= SMALLEST_MEM_UNIT) {
    chunk_t *next_chunk = chunk->next;
    chunk->next = ((void *) chunk) + sizeof(chunk_t) + size;
    chunk->next->next = next_chunk;
    chunk->next->size = chunk->size - size - sizeof(chunk_t);
    chunk->size = size;
  }
  disable_bit(&chunk->metadata, FREE_BIT);
  return ((void *) chunk) + sizeof(chunk_t);
}

void print_mem_list(void *scratchpad) {
  chunk_t *chunk = scratchpad;

  while (chunk != NULL) {
    printf("At chunk %d.  Size %d. %s\n", (char *) chunk - (char *) scratchpad,
      chunk->size, (get_bit(chunk->metadata, FREE_BIT)) ? "Free." : "Occupied.");
    if (chunk->next == NULL) {
      printf("Final chunk.\n");
    }
    chunk = chunk->next;
  }
}

void scratch_free(void *scratchpad, void *ptr) {
  if (ptr == NULL)
    return;

  chunk_t *chunk = ptr - sizeof(chunk_t);

  enable_bit(&chunk->metadata, FREE_BIT);

  // If you're freeing a chunk before an already free chunk, compact them.
  if (chunk->next != NULL && get_bit(chunk->next->metadata, FREE_BIT)) {
    chunk_t *next_chunk = chunk->next;
    chunk->next = next_chunk->next;
    chunk->size += next_chunk->size + sizeof(chunk_t);
  }
}
