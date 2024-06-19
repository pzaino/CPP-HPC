# CPP HPC

## Introduction

This repository contains libraries for high performance computing in C++ of my creation for my own projects.

The reason I did this is to have a set of libraries that I can use in my projects that are optimized for performance, that I can trust and that are split into small libraries that I can use independently. To keep my project's binaries small, I don't want to include a whole library just to use a small part of it.

Apart from this and the fact that the API is consistent through all the different
implementations (OpenMP, OpenCL, CUDA and SIMD), there is nothing special about this library. It is just a collection of libraries that I have created for my own projects.

You should be able to mix and match the different implementations in your project. For example, you can use the OpenMP implementation of one library and the CUDA of
another. You can also replace them to test which one is faster for your specific use case. The API is the same for all implementations, so you can easily switch between them.

## How to build

This project uses CMake to build. To build the project, you need to have CMake installed. You also need to have the OpenMP, OpenCL and CUDA libraries installed on your system if you want to build the OpenMP, OpenCL and CUDA implementations respectively.

This project uses googletest (gtest) for testing. If you want to build the tests, you need to have gtest installed on your system.

Installign gtest on Ubuntu:

```bash
sudo apt-get install libgtest-dev
cd /usr/src/googletest/googletest
sudo mkdir build
cd build
sudo cmake ..
sudo make
sudo cp libgtest* /usr/lib/
```

Install gtest on MacOS:
```
brew install googletest
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
./test/cpp_hpc_test
```

Have fun!
