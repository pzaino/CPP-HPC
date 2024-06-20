__kernel void dijkstra(
    __global const int* graph, 
    __global int* dist, 
    __global bool* sptSet, 
    const int n) 
{
    int count = get_global_id(0);
    
    if (count < n - 1) {
        int u = -1;
        int min = INT_MAX;

        for (int v = 0; v < n; v++) {
            if (!sptSet[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        if (u != -1) {
            sptSet[u] = true;

            for (int v = 0; v < n; v++) {
                if (!sptSet[v] && graph[u * n + v] && dist[u] != INT_MAX && dist[u] + graph[u * n + v] < dist[v]) {
                    dist[v] = dist[u] + graph[u * n + v];
                }
            }
        }
    }
}
