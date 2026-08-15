# My Data Structures Final Project - On-disk tensors with MPHF (minimal perfect hash function)

Welcome to Joe's (my) final project. For my final project I chose to implement a minimal perfect hash function, with the purpose of storing tensor data on ones disk. In proposing this project, my goal with this data structure was to optimize lookup time for neural network structures between the disk and active memory to optimize utilization of active memory (in RAM or VRAM). 

# What is the goal of this project?

The goal of this project is to create a viable data structure to store large structures of premapped vectors onto a disk in a minimal perfect hash map. 

`A note:` in my proposal I referenced LLMs quite a bit and how this is a good use case for large language models. While this has potential to be used for these kinds of models because they use Tensors heavily, **this is not the goal of this project**. The goal of this project is to create a general minimal perfect hash function structure that is memory efficient, and apply it to the concept of tensors as the data type stored. In this project demo, we are not performing any LLM inference, we will only showcase the memory and speed benefits, of using a hash map structure such as this.

Many neural network data structures today use tensors quite frequently for inference. 

# How to run this project

To run this project, there is a simple main app that you can run through cmake.
In order to run, use the following commands:

mkdir build && cd build

cmake ..

make

./tensor_mphf


This project was meant to be an experiment to measure the potential benifits of using a MPHF structure to improve memory and data round trip time from disk to RAM and vice versa. What I found and what you will see is that there are some unexpected benifits, as well as unexpected downsides to using MPHF for this purpose.

## References

In this project I used quite a few others work to implement the actually hashing and bit compacting mechanisms.

first, Rank9 is an algorithm written by Sebastiano Vigna, you can view his implementation here, it is very similar to mine (I adapted to be inside my bitvector structure): https://github.com/vigna/sux

second, for the actual heavy lifting of the MPHF mechanism, I referenced BBHash for this project, the implementation is found here: https://github.com/rizkg/BBHash

Additionally, the mixing algorithm utilizes splitmix64, which can be found here: https://github.com/indiesoftby/defold-splitmix64

As well as FNV-a1 hashing for prehashing strings to uint64 https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

All of the logic for bitvector as well as bit counts can be found in the README.md under bitvector.

Many of the concepts in this project go a bit above what we studied in this course, so I had to do a fair amount of extra research when building this out, for this I used extranious sources from the internet, as well as LLMs to help me learn the concepts and build out the algorithms.