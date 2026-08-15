#include "tensormap.h"
#include <iostream>


// constructor implementation
TensorMap::TensorMap(const std::vector<std::string>& keys, std::vector<TensorData> tensors, 
    double gamma, uint64_t seed) {
        // we first need to prehash strings keys into 64 bit integers
        if (keys.size() != tensors.size()) return;
        std::vector<uint64_t> prehashed_keys;
        prehashed_keys.reserve(keys.size());
        
        // looping and prehashing each one
        for (int i=0; i<keys.size(); i++) {
            prehashed_keys.push_back(fnv1a_64(keys[i].data(), keys[i].size()));
        }

        // then we can construct mphf
        mphf = MPHF(prehashed_keys, gamma, seed);
        
        // finally, we move values into the tensor map, 
        // first we resize values
        values.resize(keys.size());

        // then at step, perform an mphf lookup on the existing key, 
        // and move the tensor to that slot
        for (size_t i = 0; i < keys.size(); ++i)
            values[mphf.lookup(prehashed_keys[i])] = std::move(tensors[i]);
}

TensorMap::~TensorMap() {
}

TensorData TensorMap::fetchTensor(std::string key) {
    uint64_t idx = mphf.lookup(fnv1a_64(key.data(), key.size()));
    return values[idx];
}

size_t TensorMap::size() {
    return mphf.size();
}

void TensorMap::getBits() {
    std::cout << "MPHF Total nBits: " << mphf.bit_count() << std::endl;
    std::cout << "MPHF Bits per key: " << mphf.bits_per_key() << std::endl;

    size_t tensor_bits = values.size() * sizeof(TensorData) * CHAR_BIT;

    std::cout << "Tensor Map total size: " << tensor_bits + mphf.bit_count() << std::endl;
}