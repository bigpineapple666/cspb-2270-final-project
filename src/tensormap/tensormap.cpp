#include "tensormap.h"
#include <tensor/tensor.h>
#include <hash/hash.h>



TensorMap::TensorMap(const std::vector<std::string>& keys, std::vector<TensorData> tensors, 
    double gamma = 2.0, uint64_t seed = 0x9E3779B97F4A7C15ULL) {
        // we first need to prehash strings keys into 64 bit integers
        std::vector<uint64_t> prehashed_keys(keys.size());

        // looping and prehashing each one
        for (int i=0; i<keys.size(); i++) {
            prehashed_keys.push_back(fnv1a_32(keys[i].data(), keys[i].size()));
        }

        // then we can construct mphf
        mphf = new MPHF(prehashed_keys, gamma, seed);
        
        // store values
        values = tensors
}

TensorMap::~TensorMap() {

}

TensorData* TensorMap::fetchTensor(std::string key) {

}

size_t TensorMap::size() {

}