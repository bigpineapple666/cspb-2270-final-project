#include <tensor/tensor.h>
#include <tensormap/tensormap.h>
#include <vector>
#include <iostream>

using namespace std;

void basic_tensor_test1() {
    cout << "Tensor data init" << endl;
    TensorData tensor({1,2,3});
    tensor.print();
    cout << "Complete!" << endl;
    cout << "=========================================" << endl;
    cout << endl;
}

void mphf_tensor_small_test() {
    
}

int main(){
    // tensor data
    basic_tensor_test1();
    return 0;
}