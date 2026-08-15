# My Data Structures Final Project - On-disk tensors with MPHF (minimal perfect hash function)

Welcome to Joe's (my) final project. For my final project I chose to implement a minimal perfect hash function, with the purpose of storing tensor data on ones disk. In proposing this project, my goal with this data structure was to optimize lookup time for neural network structures between the disk and active memory to optimize utilization of active memory (in RAM or VRAM). 

# What is the goal of this project?

The goal of this project is to create a viable data structure to store large structures of premapped vectors in a minimal perfect hash map. Minimal perfect hash maps excel at both lookup speed and most importantly **memory optimization**. In this project I will show that with BBHash, tensor lookup keys can be optimized to as little as 3 bits! Which is very small and great in terms of memory savings at scale.

`A note:` in my proposal I referenced LLMs quite a bit and how this is a good use case for large language models. While this has potential to be used for these kinds of models because they use Tensors heavily, **this is not the goal of this project**. The goal of this project is to create a general minimal perfect hash function structure that is memory efficient, and apply it to the concept of tensors as the data type stored. In this project demo, we are not performing any LLM inference, we will only showcase the memory and speed benefits of using a perfect hash map with larger amounts of tensor data.

While this data structure does not allow you to write to the disk directly, I felt that the goal of writing this to a disk and performing experiments was out of scope, especially after realizing the complexity of MPHF structures alone, so I did not implement this. However, this capability is easily extendable.


# How to run this project

To run this project, there is a simple main app that you can run through cmake.
In order to run, use the following commands:

mkdir build && cd build

cmake ..

make

./tensor_mphf

This application runs a suite of benchmarks where you can play with how keys in our minimal perfect hash map optimize on memory at scale, and compare lookup times on MPHF vs an unordered map and a linear search on the same data corpus.

## Results

When running the app, you will see our findings are that while minimal perfect hash functions is a little slower than unordered map on average, for just linear scanning it is obviously much faster. In practice I think unordered map simulates most go-to behavior in hardware so the conclusion is, in terms of speed of lookup minimal perfect hash function is a little slower, which is not what I had initially hypothesised in my proposal.

However, you can also see in the benchmark across large corposes of keys, we do get down to 3 bits per key! Which in terms of memory is a pretty big win vs 40 bits per key for a 5 letter word, or even integers alone. This shows some promise in terms of memory optimization and I hope in the future I can continue to research this area, especially on an actual LLM with true production grade tensor volumes and keys.

## References

In this project I used quite a few others work to implement the actually hashing and bit compacting mechanisms.

first, Rank9 is an algorithm written by Sebastiano Vigna, you can view his implementation here, it is very similar to mine (I adapted to be inside my bitvector structure): https://github.com/vigna/sux

second, for the actual heavy lifting of the MPHF mechanism, I referenced BBHash for this project, the implementation is found here: https://github.com/rizkg/BBHash

Additionally, the mixing algorithm utilizes splitmix64, which can be found here: https://github.com/indiesoftby/defold-splitmix64

As well as FNV-a1 hashing for prehashing strings to uint64 https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

All of the logic for bitvector as well as bit counts can be found in the README.md under bitvector.

Many of the concepts in this project go a bit above what we studied in this course, so I had to do a fair amount of extra research when building this out, for this I used extranious sources from the internet, as well as LLMs to help me learn the concepts and build out the more advanced algorithms, including hash functions cited above.

I also used LLMs to help me create some visual aids in the readme docs to help others grasp the more complex ideas with this data structure.