// this header file will house all data structure information around tensors
// What is this module for?
// Tensor is the data structure we will be using to store tensor data in our mphf structure
#pragma once
#include <vector>

// after doing some research, I found a simple and very interesting technique
// for implementing tensors, you just treat the whole thing as one flat array!
// crazy!! but is definetly the most space efficient way to do this, then
// we just modify the functions around it
struct TensorData {
    std::vector<float> data;   
    std::vector<size_t> shape;    

    // constructor just makes a tensor of zeros
    TensorData() = default;
    TensorData(std::vector<size_t> shape);

    // print tensor data
    void print();
};
