#include <tensor/tensor.h>
#include <tensormap/tensormap.h>
#include <vector>
#include <iostream>

using namespace std;

int main(){
    // tensor data
    cout << "Tensor data init" << endl;
    vector<int> tensor_dim = {5,4,1024};
    TensorData* t_data = init_tensor_zeros(tensor_dim);
    print_tensor_data(t_data);
    cout << "Complete!" << endl;
    cout << "=========================================" << endl;
    cout << endl;
    return 0;
}