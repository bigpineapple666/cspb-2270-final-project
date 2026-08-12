// implementation of bitvector
#include "bitvector.h"

// constructor, resize and set bits
BitVector::BitVector(size_t n){
    a.resize((n + 63) >> 6, 0);
    nbits = n;
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