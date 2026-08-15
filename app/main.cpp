#include <tensor/tensor.h>
#include <tensormap/tensormap.h>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <unordered_map>
#include <utility>
#include <iomanip>

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

// prints a banner so each part of the demo is easy to pick out
void section(const string& title) {
    cout << endl;
    cout << "=============================================================" << endl;
    cout << "  " << title << endl;
    cout << "=============================================================" << endl;
}

// prints a shape the way we write it everywhere else, [a, b, c]
void print_shape(const vector<size_t>& shape) {
    cout << "[";
    for (size_t i = 0; i < shape.size(); i++) {
        cout << shape[i];
        if (i + 1 < shape.size()) cout << ", ";
    }
    cout << "]";
}

void basic_tensor_test1() {
    section("Tensor initialization");

    cout << "  Creating a tensor ... ";
    TensorData tensor({1,2,3});
    cout << "done" << endl;

    cout << "  Result: ";
    tensor.print();
}

void mphf_tensor_demo() {

    // initizalization
    const int SIZE = 5;
    vector<size_t> tensor_shape = {2<<2, 2<<10, 2<<10};

    section("MPHF tensor map demo");

    cout << "  Generating " << SIZE << " random keys ... " << flush;
    vector<string> keys = random_keys(SIZE, 7);
    cout << "done" << endl;

    cout << "  Allocating " << SIZE << " tensors of shape ";
    print_shape(tensor_shape);
    cout << endl;

    // first allocate all tensor data
    vector<TensorData> d;
    d.reserve(SIZE);

    // then initialize via emplace_back, one hash per tensor as a progress bar
    cout << "    " << flush;
    for (int j=0; j<SIZE; j++) {
        d.emplace_back(tensor_shape);
        cout << "#" << flush;
    }
    cout << " done" << endl;

    cout << "  Building tensor map with MPHF ... " << flush;
    TensorMap* map (new TensorMap(keys, d));
    cout << "done" << endl;
    cout << "  Keys in map: " << map->size() << endl;

    // test fetching one of these tensors
    cout << endl;
    cout << "  Fetching key \"" << keys[0] << "\" ... " << flush;
    TensorData testfetch = map->fetchTensor(keys[0]);
    cout << "done" << endl;
    cout << "  Result: ";
    testfetch.print();

    cout << endl;
    cout << "  Memory footprint (the main point of this structure):" << endl;
    map->getBits();

    delete map;
}

// times a block of work and reports the average nanoseconds per operation
template <typename F>
double ns_per_op(size_t n_ops, F&& work) {
    auto start = chrono::steady_clock::now();
    work();
    auto end = chrono::steady_clock::now();
    return chrono::duration<double, nano>(end - start).count() / double(n_ops);
}

void fetch_benchmark() {
    // number of keys in the map, and how many times we sweep the whole key set
    const size_t N = 20000;
    const size_t REPS = 10;

    // the linear scan is O(N) per fetch, so it only gets a small sample of
    // lookups, otherwise this benchmark would take minutes to finish
    const size_t SCAN_LOOKUPS = 500;

    // deliberately tiny tensors, see note below
    const vector<size_t> shape = {2, 2};

    section("Fetch benchmark");

    cout << "  Building " << N << " keys and tensors ... " << flush;

    vector<string> keys = random_keys(N, 7);
    vector<TensorData> d;
    d.reserve(N);
    for (size_t i = 0; i < N; ++i) d.emplace_back(shape);

    // our structure
    TensorMap tmap(keys, d);

    // baseline 1: unstructured, just two parallel vectors scanned start to end
    vector<pair<string, TensorData>> flat;
    flat.reserve(N);
    for (size_t i = 0; i < N; ++i) flat.emplace_back(keys[i], d[i]);

    // baseline 2: the standard library hash map, for a fairer comparison
    unordered_map<string, TensorData> umap;
    umap.reserve(N);
    for (size_t i = 0; i < N; ++i) umap[keys[i]] = d[i];

    // volatile so the compiler cannot decide the fetches are pointless and
    // delete the loops we are trying to measure
    volatile float sink = 0.0f;

    cout << "done" << endl;
    cout << "  Running lookups ... " << flush;

    double mphf_ns = ns_per_op(N * REPS, [&] {
        for (size_t r = 0; r < REPS; ++r)
            for (size_t i = 0; i < N; ++i)
                sink = sink + tmap.fetchTensor(keys[i]).data[0];
    });

    // note the copies below. fetchTensor hands back a TensorData by value, so
    // every MPHF fetch pays for a copy. the baselines copy too, otherwise we
    // would be timing our copy against their pointer and calling it a result
    double umap_ns = ns_per_op(N * REPS, [&] {
        for (size_t r = 0; r < REPS; ++r)
            for (size_t i = 0; i < N; ++i) {
                TensorData t = umap.at(keys[i]);
                sink = sink + t.data[0];
            }
    });

    double scan_ns = ns_per_op(SCAN_LOOKUPS, [&] {
        for (size_t i = 0; i < SCAN_LOOKUPS; ++i) {
            // spread the sampled keys across the whole set so we are not
            // always hitting the front of the vector
            const string& target = keys[(i * N) / SCAN_LOOKUPS];
            for (size_t j = 0; j < flat.size(); ++j) {
                if (flat[j].first == target) {
                    TensorData t = flat[j].second;
                    sink = sink + t.data[0];
                    break;
                }
            }
        }
    });

    cout << "done" << endl;
    cout << endl;

    cout << "  Average time per fetch:" << endl;
    cout << fixed << setprecision(1) << left;
    cout << "    " << setw(26) << "Unstructured linear scan" << setw(10) << right << scan_ns << " ns" << left << endl;
    cout << "    " << setw(26) << "std::unordered_map"       << setw(10) << right << umap_ns << " ns" << left << endl;
    cout << "    " << setw(26) << "TensorMap (MPHF)"         << setw(10) << right << mphf_ns << " ns" << left << endl;
    cout << endl;

    // ratios above 1 mean MPHF won, below 1 mean it lost, so say which
    cout << "  MPHF compared to:" << endl;
    cout << setprecision(2);
    for (auto [label, other_ns] : {pair{"linear scan", scan_ns}, pair{"unordered_map", umap_ns}}) {
        double ratio = other_ns / mphf_ns;
        cout << "    " << setw(26) << label
             << (ratio >= 1.0 ? ratio : 1.0 / ratio) << "x "
             << (ratio >= 1.0 ? "faster" : "slower") << endl;
    }
    cout << defaultfloat << right;
}

int main(){
    // tensor data
    mphf_tensor_demo();
    fetch_benchmark();
    return 0;
}