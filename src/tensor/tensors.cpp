#include "tensor.h"
#include <vector>
#include <random>

// Initialization of tensor data that = zeros
TensorData* init_tensor_zeros(std::vector<int> dimensions){
    TensorData* t_data(new TensorData);
    // set dimensions
    t_data->dims = dimensions;
    std::vector<float> v;
    for (int i = 0; i<t_data->dims.size(); i++) {
        for (int j=0; j<t_data->dims[i]; j++) {
            v = {};
            v.push_back(0.0);
        }
        t_data->t.push_back(v);
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