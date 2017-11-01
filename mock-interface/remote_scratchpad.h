#pragma once

#include <unistd.h>
#include <inttypes.h>

typedef uint64_t block_id_t;

void *rmem;
size_t rmem_block_size;
useconds_t rmem_get_delay, rmem_set_delay;

// Initialize n block_size-size blocks of 'remote memory'
void init_remote_memory(size_t block_size, size_t n, useconds_t get_delay,
  useconds_t set_delay);
void destroy_remote_memory();

// The remote memory interface defined by
// https://docs.google.com/document/d/1aTXO8ZXkvyirGcrtxPjf2cqomyVSj6OMbPk14Id2mag

// RDMA
void remote_get(block_id_t src_block_id, void *dst, size_t n);
void remote_set(void *src, block_id_t dst_block_id, size_t n);

// Scratchpad initialization
void *create_scratchpad(size_t size);
void destroy_scratchpad(void *scratchpad);

// Scratchpad malloc (for convenience)
void *init_scratchpad_malloc(void *scratchpad);
void *scratch_malloc(void *scratchpad, size_t size);
void *scratch_realloc(void *scratchpad, size_t size);
void scratch_free(void *ptr, void *scratchpad);
