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

#include <CL/opencl.hpp>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

#include "dijkstra.hpp"

// Function to load the OpenCL kernel source code
std::string loadKernelSource(const std::string &filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

template <typename T>
std::vector<int> dijkstra(const std::vector<std::vector<T>>& graph, int src) {
    int V = graph.size();
    std::vector<int> dist(V, std::numeric_limits<int>::max());
    std::vector<int> sptSet(V, 0);
    dist[src] = 0;

    // Get all platforms (drivers)
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);
    if (allPlatforms.empty()) {
        std::cout << " No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform platform = allPlatforms[0];

    // Get default device of the default platform
    std::vector<cl::Device> allDevices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);
    if (allDevices.empty()) {
        std::cout << " No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device device = allDevices[0];

    cl::Context context({device});

    cl::Program::Sources sources;

    // Kernel code
    std::string kernelCode = loadKernelSource("dijkstra_kernel.cl");
    sources.push_back({kernelCode.c_str(), kernelCode.length()});

    cl::Program program(context, sources);
    if (program.build({device}) != CL_SUCCESS) {
        std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
        exit(1);
    }

    cl::Buffer bufferGraph(context, CL_MEM_READ_ONLY, sizeof(T) * V * V);
    cl::Buffer bufferDist(context, CL_MEM_READ_WRITE, sizeof(int) * V);
    cl::Buffer bufferSptSet(context, CL_MEM_READ_WRITE, sizeof(int) * V);

    cl::CommandQueue queue(context, device);

    queue.enqueueWriteBuffer(bufferGraph, CL_TRUE, 0, sizeof(T) * V * V, &graph[0][0]);
    queue.enqueueWriteBuffer(bufferDist, CL_TRUE, 0, sizeof(int) * V, dist.data());
    queue.enqueueWriteBuffer(bufferSptSet, CL_TRUE, 0, sizeof(int) * V, sptSet.data());

    cl::Kernel kernel(program, "dijkstra");
    kernel.setArg(0, bufferGraph);
    kernel.setArg(1, bufferDist);
    kernel.setArg(2, bufferSptSet);
    kernel.setArg(3, V);

    for (int count = 0; count < V - 1; count++) {
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(V), cl::NullRange);
        queue.finish();

        queue.enqueueReadBuffer(bufferDist, CL_TRUE, 0, sizeof(int) * V, dist.data());
        queue.enqueueReadBuffer(bufferSptSet, CL_TRUE, 0, sizeof(int) * V, sptSet.data());
    }

    return dist;
}

// Explicit template instantiation
template std::vector<int> dijkstra<int>(const std::vector<std::vector<int>>&, int);
template std::vector<int> dijkstra<float>(const std::vector<std::vector<float>>&, int);
template std::vector<int> dijkstra<double>(const std::vector<std::vector<double>>&, int);
template std::vector<int> dijkstra<long>(const std::vector<std::vector<long>>&, int);
