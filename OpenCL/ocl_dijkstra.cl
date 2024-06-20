__kernel void dijkstra(__global const int* graph, __global int* dist, __global int* sptSet, const int V) {  // Changed bool to int
    int i = get_global_id(0);
    if (i < V) {
        int min = INT_MAX;
        int min_index;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }

        sptSet[min_index] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[min_index * V + v] && dist[min_index] != INT_MAX && dist[min_index] + graph[min_index * V + v] < dist[v]) {
                dist[v] = dist[min_index] + graph[min_index * V + v];
            }
        }
    }
}
