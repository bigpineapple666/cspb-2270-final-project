# The Tensor Module

This is a simple structure to store tensor data. One thing that is interesting about tensors is that while the data is meant to represent a multidimensional mathematical structure, the data structure implementation of this is fairly straighforward. 

Since this is just the object that we store in our tensor map and the goal of this project is to showcase minimal perfect hash map, this TensorData structure is just here to serve as our "Node" for something such as a hashmap. Since it is a part of the background use case, I chose to build it.

Tensors in this project are only initialized and used as nodes, no operations are performed with them. I wanted to show that my hash map has the capability to store dense tensor data, so in this implementation we initialize tensors with random uniform floating point numbers, and then those tensors are stored in the minimal perfect hash map.