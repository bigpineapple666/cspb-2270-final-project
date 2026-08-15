// header for minimal perfect hash function

#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include <bitvector/bitvector.h>

// skeleton MPHF
class MPHF {

// public functions
public:
    // MPHF requires levels for bits to entirely reduce collisions
    // this defines specifically the maximum number of levels
    static constexpr unsigned MAX_LEVELS = 25;

    // constructors
    MPHF() = default;
    MPHF(const std::vector<uint64_t>& keys,
         double __gamma = 2.0,
         uint64_t __seed = 0x9E3779B97F4A7C15ULL);
    
    // lookup a key
    uint64_t lookup(uint64_t key);
    
    // get the total number of keys
    size_t size();

    // return the tol
    size_t levels();
    size_t bit_count();
    double bits_per_key();

    static uint64_t mix(uint64_t key, uint64_t s);


private:
    BitVector bv;
    size_t nkeys;                       
    std::vector<size_t> level_offsets;     
    std::vector<size_t> level_sizes;       
    std::unordered_map<uint64_t, uint64_t> fallback;  
    uint64_t seed  = 0;
    double gamma = 2.0;
    
    void build(const std::vector<uint64_t>& keys);
};

// prehashing function, not a class member, just for preprocessing
uint64_t fnv1a_64(const void* data, size_t len);