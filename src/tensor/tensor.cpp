#include "tensor.h"
#include <iostream>
#include <random>

// Initialization of tensor data with random values

TensorData::TensorData(std::vector<size_t> __shape){

    // transfer data into shape
    shape = std::move(__shape);

    // calculate the tensor size, which is just all dimensions in shape multiplied
    size_t num_elements = 1;
    for (int i=0; i<shape.size(); i++) num_elements*=shape[i];

    // fill with random floats in [-0.5, 0.5)
    // the generator is static so we seed it once and keep drawing from it,
    // rather than reseeding on every tensor we build
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

    data.resize(num_elements);
    for (size_t i = 0; i < num_elements; i++) data[i] = dist(gen);
}

void TensorData::print() {
    // cout the shape only
    std::cout << "shape = [";
    for (size_t i = 0; i < shape.size(); i++) {
        std::cout << shape[i];
        if (i + 1 < shape.size()) std::cout << ", ";
    }
    std::cout << "], " << data.size() << " elements" << std::endl;
}