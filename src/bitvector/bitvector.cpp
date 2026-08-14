// implementation of bitvector
#include "bitvector.h"
#include <bit>
#include <cstdint>

// constructor, resize and set bits
// using the creator Vigna's implementation of rank9:
// https://github.com/vigna/sux/blob/master/sux/bits/Rank9.hpp
BitVector::BitVector(size_t n){
    bits.resize((n + 63) >> 6, 0);
    nbits = n;
    
}

// setter
void BitVector::set(size_t i){
    // bitwise OR
    bits[i >> 6] |= uint64_t(1) << (i & 63);
}

// clear vec
void BitVector::clear(size_t i){
    // bitwise AND
    bits[i >> 6] &= ~(uint64_t(1) << (i & 63));
}

// getter for individual bits
bool BitVector::get(size_t i){
    return (bits[i >> 6] >> (i & 63)) & 1;
}

// get nbits
size_t BitVector::getNBits(){
    return nbits;
}

// rank9 implementation
// using the creator Vigna's implementation of rank9:
// https://github.com/vigna/sux/blob/master/sux/bits/Rank9.hpp
void BitVector::build_rank9(){
    const uint64_t num_words = (nbits + 63) / 64;
	const uint64_t num_counts = ((nbits + 64 * 8 - 1) / (64 * 8)) * 2;

    rank9_idx.clear();
    rank9_idx.assign(num_counts + 2, 0);

    size_t nones = 0;
    uint64_t pos = 0;
    for (uint64_t i = 0; i < num_words; i += 8, pos += 2) {
        rank9_idx[pos] = nones;
        nones += std::popcount(bits[i]);
        for (int j = 1; j < 8; j++) {
            rank9_idx[pos + 1] |= (nones - rank9_idx[pos]) << 9 * (j - 1);
            if (i + j < num_words) nones += std::popcount(bits[i + j]);
        }
    }

    rank9_idx[num_counts] = nones;

    if(nones > nbits) return;
}

uint64_t BitVector::rank9(uint64_t k) {
    const uint64_t word = k / 64;
    const uint64_t block = word / 4 & ~1;
    const int offset = word % 8 - 1;
    return rank9_idx[block] + (rank9_idx[block + 1] >> (offset + (offset >> (sizeof offset * 8 - 4) & 0x8)) * 9 & 0x1FF) + std::__popcount(bits[word] & ((1ULL << k % 64) - 1));
}