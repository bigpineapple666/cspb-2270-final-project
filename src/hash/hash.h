// header for minimal perfect hash function

#include "bitvector.h"

BitVector::BitVector(size_t n){
    vec.resize((n+63) >> 6, 0);
    nbits = n;
}


void BitVector::set(size_t i){

}

void BitVector::clear(size_t i){

}

bool BitVector::get(size_t i){

}