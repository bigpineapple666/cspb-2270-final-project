#include <tensor/tensor.h>
#include <tensormap/tensormap.h>
#include <vector>
#include <iostream>
#include <random>

using namespace std;


// function used for generating random keys
vector<string> random_keys(size_t n, size_t len, uint64_t seed = 42) {
    // define alphabet
    static constexpr char alphabet[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    // -1 drops the null character'\0'
    constexpr size_t alpha_size = sizeof(alphabet) - 1;   
    mt19937_64 rng(seed);
    uniform_int_distribution<size_t> pick(0, alpha_size - 1);

    vector<string> keys;
    keys.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        string s(len, '\0');
        for (size_t j = 0; j < len; ++j) s[j] = alphabet[pick(rng)];
        keys.push_back(std::move(s));
    }
    return keys;
}

void basic_tensor_test1() {
    cout << "Tensor data init" << endl;
    TensorData tensor({1,2,3});
    tensor.print();
    cout << "Complete!" << endl;
    cout << "=========================================" << endl;
    cout << endl;
}

void mphf_tensor_small_test() {

    // initizalization
    const int SIZE = 500;
    
    
    cout << "Initializing " << SIZE << "keys... " << endl;
    vector<string> keys = random_keys(SIZE, 7);
    cout << "Done." <<endl;
    // print initialization message
    cout << "Initializing " << SIZE << " Tensors of size: ";
    vector<size_t> tensor_shape = {2<<2, 2<<10, 2<<10};
    vector<TensorData> d;
    for (int i=0; i<tensor_shape.size(); i++) cout << tensor_shape[i] << " ";
    cout << endl;

    // first allocate all tensor data
    d.reserve(SIZE);

    // then initialize via emplace_back
    d.emplace_back(tensor_shape);

    cout << "Done." <<endl;

    cout << "Initializing custom tensor map with MPHF" << endl;
    // init tensor map
    TensorMap* map (new TensorMap(keys, d));
    cout << "Size: " << map->size() << endl;
    cout << "Done." << endl;
    // test fetching one of these tensors

    cout << "Testing fetch of tensor..." << endl;
    TensorData testfetch = map->fetchTensor(keys[0]);
    testfetch.print();
    cout << "Done." << endl;
}

int main(){
    // tensor data
    mphf_tensor_small_test();
    return 0;
}