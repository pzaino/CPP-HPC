// Copyright 2023 Paolo Fabio Zaino
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <CL/cl.hpp>
#include <vector>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ocl_dijkstra.hpp"

int minDistance(const std::vector<int>& dist, const std::vector<bool>& sptSet) 
{
    int min = INT_MAX, min_index = -1;

    for (size_t v = 0; v < dist.size(); v++)
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }

    return min_index;
}

template <typename T>
std::vector<int> dijkstra(const std::vector<std::vector<T>>& graph, int src) 
{
    int n = graph.size();
    if (n == 0) return {};

    std::vector<int> dist(n, INT_MAX);
    std::vector<bool> sptSet(n, false);

    dist[src] = 0;

    // Convert graph to 1D array for OpenCL
    std::vector<int> graph_flat(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            graph_flat[i * n + j] = graph[i][j];
        }
    }

    // Load kernel source
    std::ifstream kernelFile("dijkstra.cl");
    std::stringstream kernelSource;
    kernelSource << kernelFile.rdbuf();
    std::string src_str = kernelSource.str();
    const char* source = src_str.c_str();

    // Set up OpenCL
    cl::Platform platform = cl::Platform::getDefault();
    cl::Device device = platform.getDefaultDevice();
    cl::Context context(device);
    cl::Program program(context, source);
    program.build();

    cl::Buffer bufferGraph(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * graph_flat.size(), graph_flat.data());
    cl::Buffer bufferDist(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * dist.size(), dist.data());
    cl::Buffer bufferSptSet(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(bool) * sptSet.size(), sptSet.data());

    cl::Kernel kernel(program, "dijkstra");
    kernel.setArg(0, bufferGraph);
    kernel.setArg(1, bufferDist);
    kernel.setArg(2, bufferSptSet);
    kernel.setArg(3, n);

    cl::CommandQueue queue(context, device);
    
    // Execute the kernel
    for (int count = 0; count < n - 1; ++count) {
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(n), cl::NullRange);
        queue.finish();
    }

    // Read the results
    queue.enqueueReadBuffer(bufferDist, CL_TRUE, 0, sizeof(int) * dist.size(), dist.data());

    return dist;
}

// Explicit template instantiation
template std::vector<int> dijkstra<int>(const std::vector<std::vector<int>>&, int);
template std::vector<int> dijkstra<float>(const std::vector<std::vector<float>>&, int);
template std::vector<int> dijkstra<double>(const std::vector<std::vector<double>>&, int);
template std::vector<int> dijkstra<long>(const std::vector<std::vector<long>>&, int);
template std::vector<int> dijkstra<unsigned int>(const std::vector<std::vector<unsigned int>>&, int);
