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

#include <iostream>
#include <vector>
#include <limits.h>
#include <omp.h>

#include "omp_dijkstra.hpp"

#define V 9

template <typename T>
int minDistance(const std::vector<T>& dist, const std::vector<bool>& sptSet) 
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

template <typename T>
void dijkstra(const std::vector<std::vector<T>>& graph, int src) 
{
    std::vector<int> dist(V, INT_MAX);
    std::vector<bool> sptSet(V, false);

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) 
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        #pragma omp parallel for
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    std::cout << "Vertex \t Distance from Source\n";
    for (int i = 0; i < V; i++)
        std::cout << i << " \t\t " << dist[i] << "\n";
}

// Explicit template instantiation
template int minDistance<int>(const std::vector<int>&, const std::vector<bool>&);
template void dijkstra<int>(const std::vector<std::vector<int>>&, int);

template int minDistance<float>(const std::vector<float>&, const std::vector<bool>&);
template void dijkstra<float>(const std::vector<std::vector<float>>&, int);

template int minDistance<double>(const std::vector<double>&, const std::vector<bool>&);
template void dijkstra<double>(const std::vector<std::vector<double>>&, int);

template int minDistance<long>(const std::vector<long>&, const std::vector<bool>&);
template void dijkstra<long>(const std::vector<std::vector<long>>&, int);

template int minDistance<unsigned int>(const std::vector<unsigned int>&, const std::vector<bool>&);
template void dijkstra<unsigned int>(const std::vector<std::vector<unsigned int>>&, int);
