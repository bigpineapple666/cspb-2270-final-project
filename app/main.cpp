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
    //// bitvectors
    cout << "Bitvector: testing and understanding" << endl;
    BitVector b = BitVector(2 << 7);
    b.set(65);
    for (int i=0; i < b.bits.size(); i++) {
        cout << b.bits[i] << " ";
    }
    cout << endl;
    b.build_rank9();
    cout << b.rank9(2 << 7) << endl;
    return 0;
}