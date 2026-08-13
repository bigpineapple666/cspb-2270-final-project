// implementation of bitvector
#include "bitvector.h"
#include <bit>

// constructor, resize and set bits
BitVector::BitVector(size_t n){
    a.resize((n + 63) >> 6, 0);
    nbits = n;

    rank9_idx.clear();
    rank9_idx.resize((a.size() + 3) / 4 * 2); // Rank9 layout allocation
    
    uint64_t total_ones = 0;
    for (size_t i = 0; i < a.size(); i += 4) {
        size_t idx = (i / 4) * 2;
        rank9_idx[idx] = total_ones; // Absolute count for 256-bit superblock
        
        // Precompute relative counts inside the next fields if using Vigna's packed structure
        // (Or use a simpler implementation structure like the one below)
        for(size_t j=0; j < 4 && (i+j) < a.size(); ++j) {
            total_ones += std::popcount(a[i+j]);
        }
    }
}

// setter
void BitVector::set(size_t i){
    // bitwise OR
    a[i >> 6] |= uint64_t(1) << (i & 63);
}

// clear vec
void BitVector::clear(size_t i){
    // bitwise AND
    a[i >> 6] &= ~(uint64_t(1) << (i & 63));
}

// getter
bool BitVector::get(size_t i){
    return (a[i >> 6] >> (i & 63)) & 1;
}