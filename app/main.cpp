#include <tensor/tensor.h>
#include <vector>

int main(){
    std::vector<int> tensor_dim = {512,412,1024};
    TensorData* t_data = init_tensor_zeros(tensor_dim);
    print_tensor_data(t_data);
    return 0;
}