// Copyright 2023 Safronov Mikhail
#include "task_3/safronov_dijkstra/dijkstra.h"
const int INF = INT_MAX;

int minDistance(int dist[], bool sptSet[], int n) {
    int min = INT_MAX, min_index = -1;

    for (int v = 0; v < n; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}


void dijkstra(MPI_Comm comm, int** graph, int n, int src, int* resDist) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int* dist = new int[n];
    bool* sptSet = new bool[n];

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, sptSet, n);

        sptSet[u] = true;

        for (int v = 0; v < n; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
            && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    int* allDist = new int[size * n];
    MPI_Gather(dist, n, MPI_INT, allDist, n, MPI_INT, 0, comm);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            resDist[i] = INT_MAX;
            for (int j = 0; j < size; j++) {
                resDist[i] = std::min(resDist[i], allDist[j * n + i]);
            }
        }
    }

    delete[] dist;
    delete[] sptSet;
    delete[] allDist;
}
