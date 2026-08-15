#include "tensor.h"
#include <iostream>

// Initialization of tensor data that = zeros

TensorData::TensorData(std::vector<size_t> __shape){

    // transfer data into shape
    shape = std::move(__shape);

    // calculate the tensor size, which is just all dimensions in shape multiplied
    size_t num_elements = 1;
    for (int i=0; i<shape.size(); i++) num_elements*=shape[i];

    // assign them all to 0
    data.assign(num_elements, 0.0f);
}

void TensorData::print() {
    // cout the shape only
    std::cout << "(shape = [";
    for(int i=0; i<shape.size(); i++) std::cout << shape[i] << ",";
    std::cout << "])" << std::endl;
}