# CPP HPC

## Introduction

This repository contains libraries for high-performance computing in C++, created for my own projects, or just for fun and learning.

The reason I created these libraries is to have a set of libraries optimized for performance and relatively easy to maintain, which I can trust and use independently in my projects. To keep my project's binaries small, I don't want to include a large library just to use a small part of it.

Apart from this and the fact that the API is consistent across all the different implementations (OpenMP, OpenCL, CUDA, and SIMD), there is nothing special about this library. It is just a collection of classes and routines I have created for fun, learning, and use in my own projects.

You should be able to mix and match the different implementations in your project. For example, you can use the OpenMP implementation of one library for matrix summation and the CUDA implementation of another for vector processing. You can also replace them to test which one is faster for your specific use case. The API is the same for all implementations, so you can easily switch between them.

## How to Build

This project uses CMake to build. To build the project, you need to have CMake installed. You also need to have the OpenMP, OpenCL, and CUDA libraries installed on your system if you want to build the OpenMP, OpenCL, and CUDA implementations respectively.

This project uses GoogleTest (gtest) for testing. If you want to build the tests, you need to have gtest installed on your system.

### Installing gtest on Ubuntu

```bash
sudo apt-get install libgtest-dev
cd /usr/src/googletest/googletest
sudo mkdir build
cd build
sudo cmake ..
sudo make
sudo cp libgtest* /usr/lib/
```

### Installing gtest on MacOS

```bash
brew install googletest
```

#### Installing OpenMP on Ubuntu

```bash
sudo apt-get install libomp-dev
```

#### Installing OpenCL on Ubuntu

```bash
sudo apt-get install ocl-icd-opencl-dev
```

#### Installing OpenCL on MacOS

```bash
brew install opencl-clhpp-headers
```

#### Installing CUDA on Ubuntu

```bash
sudo apt-get install nvidia-cuda-toolkit
```



To build the project, run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

To build the tests, run the following commands:

```bash
mkdir build
cd build
cmake -DBUILD_TESTS=ON ..
make
```

To run the tests, run the following command:

```bash
./build/<type>_<name>_test
```

Where `type` is the type of the implementation:

* omp  = OpenMP
* ocl  = OpenCL
* cuda = CUDA
* simd = SIMD

and `name` is the name of the library.

Have fun!
