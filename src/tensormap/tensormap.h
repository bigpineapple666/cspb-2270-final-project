#pragma once
#include <tensor/tensor.h>
#include <hash/hash.h>
#include <vector>


// definition for the tensor map data structure with MPHF

class TensorMap {

    public:
        // construtor - enter keys and tensor data
        TensorMap(const std::vector<uint64_t>& keys, std::vector<TensorData>& tensors,
           double gamma = 2.0, uint64_t seed = 0x9E3779B97F4A7C15ULL);

        // destructor
        ~TensorMap();
    
        // fetch tensor
        TensorData* fetchTensor(std::string key);
    
        // get size of table
        size_t size();

    private:
        // minimal perfect hash function
        MPHF mphf;

        // seed for mixing bits
        uint64_t seed;
        
        // tensor data
        std::vector<TensorData> tensors;

        // fingerprints
        std::vector<uint8_t> fingerprints;

}