#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "kmer_t.hpp"

int line_count(const std::string &fname) {
  FILE *f = fopen(fname.c_str(), "r");
  int n_lines = 0;
  int n_read;

  const size_t buf_size = 16384;
  char buf[buf_size];

  do {
    n_read = fread(buf, sizeof(char), buf_size, f);
    for (int i = 0; i < n_read; i++) {
      if (buf[i] == '\n') {
        n_lines++;
      }
    }
  } while (n_read != 0);
  fclose(f);
  return n_lines;
}

std::vector <kmer_t> read_kmers(const std::string &fname, int nprocs = 1, int rank = 0) {
  int num_lines = line_count(fname);
  int split = (num_lines + nprocs - 1) / nprocs;
  int start = split*rank;
  int size = std::min(split, num_lines - start);

  FILE *f = fopen(fname.c_str(), "r");
  const size_t line_len = KMER_LEN + 4;
  fseek(f, line_len*start, SEEK_SET);

  std::shared_ptr <char> buf(new char[line_len*size]);
  fread(buf.get(), sizeof(char), line_len*size, f);

  std::vector <kmer_t> kmers;

  for (int line_offset = 0; line_offset < line_len*size; line_offset += line_len) {
    char *kmer_buf = &buf.get()[line_offset];
    char *fb_ext_buf = kmer_buf + KMER_LEN+1;
    kmers.push_back(kmer_t(std::string(kmer_buf, KMER_LEN), std::string(fb_ext_buf, 2)));
  }
  fclose(f);
  return kmers;
}
