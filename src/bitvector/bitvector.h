// header file for bitvector
// in order to implement our minimal perfect hash function, 
// for fingerprinting we must use bitvectors
// this module is dedicated to building bitvectors
#pragma once
#include <cstdint>
#include <vector>

// define bitvector type
class BitVector {

    public:
        
        // constructer
        BitVector() = default;
        BitVector(size_t n);
        
        // set bit
        void set(size_t i);

        // clear bits
        void clear(size_t i);

        // get bit
        bool get(size_t i);

        // get nbits
        size_t getNBits();

        // rank9 algorithim
        void build_rank9();
        uint64_t rank9(uint64_t bit_idx);

    private:
        // bits
        std::vector<uint64_t> bits;

        // rank 9 index
        std::vector<uint64_t> rank9_idx;

        // n bits
        size_t nbits;
};

// Side note: size_t is just a larger version of unsigned int, 
// it can hold more data and in MPHF we need the space