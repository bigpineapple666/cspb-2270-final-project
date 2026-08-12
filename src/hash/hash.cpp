// hashing
// the real crux of the project, in the paper cited in 
// readme.md, there are many listed techniques for implmeneting 
// MPHF, but over large N fingerprinting is the best option

// For more on fingerprinting and the algorithm, read docs
// or the readme.md

// in MPHF, it is assumed that the data structure is already well known
// thus, we dont need traditional hash functions for this because uniqueness
// not the issue, instead we construct the whole structure all at once!

// additionally, MPHF removes the concept of nodes, instead in MPHF with
// fingerprinting we use bit vectors instead

#include <vector>


