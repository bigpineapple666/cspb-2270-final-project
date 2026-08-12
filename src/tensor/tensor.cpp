#include "tensor.h"
#include <vector>
#include <random>
#include <iostream>

// Initialization of tensor data that = zeros
TensorData* init_tensor_zeros(std::vector<int> dimensions){
    if (dimensions.size() > 3) return nullptr;

    TensorData* t_data(new TensorData);
    // set dimensions
    t_data->dims = dimensions;
    // populate tensor
    for(int i=0; i<t_data->dims[0]; i++){
        std::vector<std::vector<float>> a = {};
        for(int j=0; j<t_data->dims[1]; j++){
            std::vector<float> b = {};
            for(int k=0; k<t_data->dims[2]; k++) b.push_back(0.0);
            a.push_back(b);
        }
        t_data->t.push_back(a);
    }
    return t_data;
}

// a tensor of random numbers
TensorData* init_tensor_rand(std::vector<int> dimensions){
    TensorData* t(new TensorData);
    // set dimensions
    t->dims = dimensions;
    return t;
}

// and then as bonus points, from GGUF, a standard filetype for storing LLM tensors
TensorData* init_tensor_gguf(std::vector<int> dimensions){
    TensorData* t(new TensorData);
    // set dimensions
    t->dims = dimensions;
    
    return t;
}

// tensor data printer
void print_tensor_data(TensorData* in_tensor){
    // first print dimensions
    std::cout << "(dims=[";
    for(int i=0; i<in_tensor->dims.size(); i++)
        std::cout << in_tensor->dims[i] << ",";
    std::cout << "], ";
    // then we can just print if the vector is populated
    std::cout << "populated=";
    if (in_tensor->t.empty()) std::cout << "false";
    else std::cout << "true";
    std::cout << ")" << std::endl;
}
