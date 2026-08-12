// this header file will house all data structure information around tensors
// What is this module for?
// Tensor is the data structure we will be using to store tensor data in our mphf structure
#include <vector>

// first define the tensor type as a 3 dimensional vector
typedef  std::vector<std::vector<std::vector<float>>> tensor;

// then our data packet, this is the structure we will use to store tensor data
struct TensorData {
    // the tensor data (3 dimensional vector for LLM tensors)
    tensor t;
    // its dimensions
    std::vector<int> dims;
};

// for our first function, we will create a tensor of zeros
TensorData* init_tensor_zeros(std::vector<int> dimensions);

// a tensor of random numbers
TensorData* init_tensor_rand(std::vector<int> dimensions);

// and then as bonus points, from GGUF, a standard filetype for storing LLM tensors
TensorData* init_tensor_gguf(std::vector<int> dimensions);
