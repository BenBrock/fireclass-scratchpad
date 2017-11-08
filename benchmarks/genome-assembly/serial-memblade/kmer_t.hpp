#pragma once

#include <string>

class kmer_t {
public:
  std::string key;
  std::string fb_ext;

  kmer_t(std::string key, std::string fb_ext) :
    key(key), fb_ext(fb_ext) {}

  kmer_t(const kmer_t &kmer) {
    key = kmer.key;
    fb_ext = kmer.fb_ext;
  }

  const char &forwardExt() const {
    return fb_ext[1];
  }

  const char & backwardExt() const {
    return fb_ext[0];
  }

  std::string nextKmer() const {
    return key.substr(1, std::string::npos) + forwardExt();
  }

  void print() const {
    printf("%s %s\n", key.c_str(), fb_ext.c_str());
  }

  kmer_t() {}
};

std::string pack_kmer(const kmer_t &kmer) {
  return kmer.key + kmer.fb_ext;
}

kmer_t unpack_kmer(const std::string &kmer) {
  return kmer_t(kmer.substr(0, kmer.length()-2), kmer.substr(kmer.length()-2, 2));
}
