#pragma once

extern "C" {
  #include <remote_scratchpad.h>
}

#include <cstring>
#include <memory>
#include <map>
#include <algorithm>

class Page {
public:
  std::shared_ptr <char> data;
  uint64_t last_used;

  Page(std::shared_ptr <char> data, uint64_t last_used = 0) :
    data(data), last_used(last_used) {}
  Page() {}
};

class CachedRead {
public:
  size_t n_blocks, block_size;
  size_t begin_address = 0;
  size_t size_bytes, cache_size;
  uint64_t mem_clock = 0;

  std::map <int, Page> pages;

  CachedRead(size_t block_size, size_t n_blocks, size_t cache_size = 0) :
    n_blocks(n_blocks), block_size(block_size), cache_size(cache_size) {
      size_bytes = begin_address + n_blocks*block_size;
  }

  void read(size_t addr, size_t size, void *dst) {
    size_t end_addr = addr + size;
    for (size_t cur_addr = addr; cur_addr < end_addr; cur_addr = ((cur_addr/block_size)+1)*block_size) {
      size_t block = cur_addr / block_size;
      size_t block_end_addr = (block+1) * block_size;
      size_t cur_size = std::min(block_end_addr - cur_addr, end_addr - cur_addr);
      std::shared_ptr <char> data = get_block(block);
      memcpy((char *) dst + (cur_addr - addr),
        (char *) data.get() + (cur_addr - block*block_size), cur_size);
    }
  }

  std::shared_ptr <char> get_block(size_t block) {
    // If cache turned off, just get page.
    if (cache_size == 0) {
      std::shared_ptr <char> data(new char[block_size]);
      remote_get(block, data.get(), 1);
      return data;
    }

    if (pages.find(block) == pages.end()) {
      if (pages.size() + 1 > cache_size) {
        remove_lru();
      }
      pages[block] = Page(std::shared_ptr <char> (new char[block_size]));
      remote_get(block, pages[block].data.get(), 1);
    }
    pages[block].last_used = mem_clock++;
    return pages[block].data;
  }

  void remove_lru() {
    auto lru = std::min_element(pages.begin(), pages.end(),
      [] (std::pair <int, Page> x, std::pair <int, Page> y) {
        return x.second.last_used < y.second.last_used;
      });
    if (lru != pages.end()) {
      pages.erase(lru);
    }
  }

  void flush() {
    pages.clear();
  }
};
