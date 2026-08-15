# The Tensor Map module

The Tensor Map is the glue that brings minimal perfect hashing together with tensors. Really, there is not much in this structure that is new, the main idea is to consolidate a tensor map of some kind to an easy to access data structure, and an interface so users do not have to interface with MPHF directly. 

Each function is just an interface on top of MPHF or the vector store for our values: 

- `TensorMap()` — prehashes the string keys, builds the MPHF from them, then moves each tensor into the slot that MPHF assigns to its key.
- `fetchTensor(key)` — prehashes the key, asks MPHF for its slot, and returns the tensor sitting there.
- `size()` — passes through the key count from MPHF.
- `getBits()` — prints the MPHF bit count and bits per key, plus the size of the stored tensors for a total footprint.
