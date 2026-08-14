#include "tensormap.h"


// constructor implementation
TensorMap::TensorMap(const std::vector<std::string>& keys, std::vector<TensorData> tensors, 
    double gamma, uint64_t seed) {
        // we first need to prehash strings keys into 64 bit integers
        std::vector<uint64_t> prehashed_keys(keys.size());

        // looping and prehashing each one
        for (int i=0; i<keys.size(); i++) {
            prehashed_keys.push_back(fnv1a_32(keys[i].data(), keys.size()));
        }

        // then we can construct mphf
        mphf = new MPHF(prehashed_keys, gamma, seed);
        
        // store values
        values = tensors;
}

TensorMap::~TensorMap() {

}

TensorData* TensorMap::fetchTensor(std::string key) {

}

size_t TensorMap::size() {

}