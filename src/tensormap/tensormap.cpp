#include "tensormap.h"
#include <iostream>


// constructor implementation
TensorMap::TensorMap(const std::vector<std::string>& keys, std::vector<TensorData> tensors, 
    double gamma, uint64_t seed) {
        // we first need to prehash strings keys into 64 bit integers
        std::cout << tensors.size() << std::endl;
        if (keys.size() != tensors.size()) return;
        std::vector<uint64_t> prehashed_keys;
        prehashed_keys.reserve(keys.size());
        // looping and prehashing each one
        for (int i=0; i<keys.size(); i++) {
            prehashed_keys.push_back(fnv1a_64(keys[i].data(), keys[i].size()));
        }

        // then we can construct mphf
        mphf = MPHF(prehashed_keys, gamma, seed);
        std::cout << "after assign: " << mphf.size() << "\n";
        
        values.resize(keys.size());

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