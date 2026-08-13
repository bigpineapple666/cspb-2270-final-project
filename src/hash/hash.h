// header for minimal perfect hash function

#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include "bitvector/bitvector.h"

// skeleton MPHF
class MPHF {
public:
    static constexpr unsigned MAX_LEVELS = 25;

    MPHF() = default;
    MPHF(const std::vector<uint64_t>& keys,
         double gamma = 2.0,
         uint64_t seed = 0x9E3779B97F4A7C15ULL);

    uint64_t lookup(uint64_t key) const;

    size_t size()      const { return nkeys; }
    size_t levels()    const { return level_sizes.size(); }
    size_t bit_count() const;              // for your space measurements
    double bits_per_key() const { return double(bit_count()) / nkeys; }

private:
    BitVector bv;                         
    std::vector<size_t> level_offsets;     
    std::vector<size_t> level_sizes;       
    std::unordered_map<uint64_t, uint64_t> fallback;  
    uint64_t seed  = 0;
    double gamma = 2.0;

    static uint64_t mix(uint64_t key, uint64_t s);
    void build(const std::vector<uint64_t>& keys);
};