// implementation of bitvector
#include "bitvector.h"

// constructor, resize and set bits
BitVector::BitVector(size_t n){
    a.resize((n+63) >> 6, 0);
    nbits = n;
}