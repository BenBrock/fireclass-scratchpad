#pragma once

extern "C" {
  #include <remote_scratchpad.h>
}

#include <cstring>

class CachedRead {
public:
  size_t n_blocks, block_size;
  size_t begin_address = 0;
  size_t size_bytes;

  std::map <int, std::shared_ptr <char>> pages;

  CachedRead(size_t block_size, size_t n_blocks) :
    n_blocks(n_blocks), block_size(block_size) {
      size_bytes = begin_address + n_blocks*block_size;
  }

  void read(size_t addr, size_t size, void *dst) {
    size_t end_addr = addr + size;
    for (size_t cur_addr = addr; cur_addr < end_addr; cur_addr = ((cur_addr/block_size)+1)*block_size) {
      size_t block = cur_addr / block_size;
      size_t block_end_addr = (block+1) * block_size;
      size_t cur_size = std::min(block_end_addr - cur_addr, end_addr - cur_addr);
      if (cur_size == block_size) {
        remote_get(block, (char *) dst + (cur_addr - addr), 1);
      } else {
        std::shared_ptr <char> data(new char[block_size]);
        remote_get(block, data.get(), 1);
        memcpy((char *) dst + (cur_addr - addr),
          (char *) data.get() + (cur_addr - block*block_size), cur_size);
      }
    }
  }

  void flush() {
    pages.clear();
  }

};
