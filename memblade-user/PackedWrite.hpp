#pragma once

#include <cstdlib>
#include <memory>
#include <list>
#include <map>
#include <cstring>

extern "C" {
  #include <remote_scratchpad.h>
}

class Write {
public:
  std::shared_ptr <char> my_data;
  size_t addr, size;

  Write(size_t addr, size_t size, void *data) :
    addr(addr), size(size) {
      my_data = std::shared_ptr <char> (new char[size]);
      memcpy(my_data.get(), data, size);
    }
};

class PackedWrite {
public:
  size_t n_blocks, block_size;
  size_t begin_address = 0;
  size_t size_bytes;
  size_t max_pack;
  // block -> writes
  std::map <int, std::list <Write>> writes;

  size_t n_bytes() {
    size_t size = 0;
    for (const auto &write_it : writes) {
      for (const auto &write : write_it.second) {
        size += write.size;
      }
    }
    return size;
  }

  // Performing a packed write to a memory blade
  // of size n blocks of block_size bytes.
  PackedWrite(size_t block_size, size_t n_blocks, size_t max_pack = 1024*1024) :
    n_blocks(n_blocks), block_size(block_size), max_pack(max_pack) {
      size_bytes = begin_address + n_blocks*block_size;
  }

  PackedWrite() {}

  // TODO: Automatically issue once 'writes' becomes too large.
  void write(size_t addr, size_t size, const void *data) {
    size_t end_addr = addr + size;
    for (size_t cur_addr = addr; cur_addr < end_addr; cur_addr = ((cur_addr/block_size)+1)*block_size) {
      size_t block = cur_addr / block_size;
      size_t block_end_addr = (block+1) * block_size;
      size_t cur_size = std::min(block_end_addr - cur_addr, end_addr - cur_addr);
      writes[block].push_back(Write(cur_addr, cur_size, (char *) data + (cur_addr - addr)));
    }

    if (n_bytes() > max_pack) {
      issue();
    }
  }

  void issue() {
    for (const auto &writeList : writes) {
      std::shared_ptr <char> data(new char[block_size]);
      size_t block = writeList.first;
      size_t head_addr = block*block_size;
      remote_get(block, data.get(), 1);
      for (const auto &write : writeList.second) {
        size_t relative_addr = write.addr - head_addr;
        memcpy(data.get() + relative_addr, write.my_data.get(), write.size);
      }
      remote_set(data.get(), block, 1);
    }
    writes.clear();
  }

  void print() {
    for (const auto &writeList : writes) {
      printf("Looking at block %d\n", writeList.first);
      for (const auto &write : writeList.second) {
        printf("Write at %d (size %d)\n", write.addr, write.size);
      }
    }
  }
};
