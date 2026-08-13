// header file for bitvector
// in order to implement our minimal perfect hash function, 
// for fingerprinting we must use bitvectors
// this module is dedicated to building bitvectors
#include <cstdint>
#include <vector>
#include <bit>

// define bitvector type
struct BitVector {
    std::vector<uint64_t> a;
    std::vector<uint64_t> rank9_idx;
    size_t nbits;
    // constructer
    BitVector(size_t n);
    // operators
    void set(size_t i);
    void clear(size_t i);
    bool get(size_t i);
};

// Side note: size_t is just a larger version of unsigned int, 
// it can hold more data and in MPHF we need the space