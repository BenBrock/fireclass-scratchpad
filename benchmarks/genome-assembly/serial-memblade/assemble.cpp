#include <cstdlib>
#include <vector>
#include <list>
#include <map>
#include <numeric>
#include <memory>
#include "kmer_t.hpp"
#include "read_kmers.hpp"

extern "C" {
  #include <remote_scratchpad.h>
}

#include "CachedRead.hpp"
#include "PackedWrite.hpp"

int main(int argc, char **argv) {
  std::vector <kmer_t> kmers = read_kmers("tiny.dat");
  printf("%d kmers read.\n", kmers.size());

  size_t block_size = 4096;
  size_t n_blocks = 1000000;
  size_t size_of_kmer;
  size_of_kmer = kmers[0].key.length() + kmers[0].fb_ext.length();

  init_remote_memory(block_size, n_blocks, 1, 1);

  PackedWrite write(block_size, n_blocks);

  int kmer_loc = 0;
  std::map <std::string, int> kmer_hash;
  std::list <kmer_t> start_nodes;
  for (const auto &kmer : kmers) {
    std::string packed_kmer = pack_kmer(kmer);
    kmer_hash[kmer.key] = kmer_loc;
    write.write(kmer_loc, packed_kmer.size(), packed_kmer.c_str());
    kmer_loc += packed_kmer.size();
    if (kmer.backwardExt() == 'F') {
      start_nodes.push_back(kmer);
    }
  }
  write.issue();

  printf("Made hash table!\n");
  kmers.clear();

  std::list <std::list <kmer_t>> contigs;

  CachedRead read(block_size, n_blocks);

  for (const auto &kmer : start_nodes) {
    std::list <kmer_t> contig;
    contig.push_back(kmer);
    while (contig.back().forwardExt() != 'F') {
      size_t loc = kmer_hash[contig.back().nextKmer()];
      std::string packed_kmer(size_of_kmer, 'a');
      read.read(loc, packed_kmer.size(), packed_kmer.data());
      kmer_t my_kmer = unpack_kmer(packed_kmer);
      contig.push_back(my_kmer);
    }
    contigs.push_back(contig);
  }

  int numKmers = std::accumulate(contigs.begin(), contigs.end(), 0,
    [] (int sum, const std::list <kmer_t> &contig) {
      return sum + contig.size();
    });

  printf("StartNodes has %d elements.\n", start_nodes.size());
  printf("%d contigs with a total of %d elements (%d average)\n", contigs.size(), numKmers,
    numKmers / contigs.size());

  destroy_remote_memory();
  return 0;
}
