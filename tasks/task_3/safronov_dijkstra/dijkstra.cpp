// Copyright 2023 Safronov Mikhail
#include "task_3/safronov_dijkstra/dijkstra.h"

int minDistance(int dist[], bool sptSet[], int n) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < n; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printSolution(int dist[], int n) {
    std::cout << "Минимальные расстояния от начальной вершины до всех остальных:\n";
    for (int i = 0; i < n; i++)
        std::cout << i << ": " << dist[i] << std::endl;
}

void dijkstra(int** graph, int n, int src, int* resDist) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int INF = 99999999;
    //std::cout << rank <<  " dijkstra " << std::endl;
    int dist[n];
    bool sptSet[n];

    // Calculate the number of vertices each process will handle
    int verticesPerProcess = (n + size - 1) / size;
    int startVertex = rank * verticesPerProcess;
    int endVertex = std::min((rank + 1) * verticesPerProcess, n);

    for (int i = 0; i < n; i++) {
        dist[i] = (i == src) ? 0 : INF;
        sptSet[i] = false;
    }

    int localDist[n];
    for (int i = 0; i < n; i++) {
        localDist[i] = dist[i];
    }

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(localDist, sptSet, n);
        sptSet[u] = true;

        for (int v = startVertex; v < endVertex; v++) {
            if (!sptSet[v] && graph[u][v] && localDist[u] != INF &&
                localDist[u] + graph[u][v] < localDist[v]) {
                localDist[v] = localDist[u] + graph[u][v];
            }
        }
        //std::cout << "before gather" << std::endl;
        MPI_Allgather(localDist + startVertex, verticesPerProcess, MPI_INT, dist, verticesPerProcess, MPI_INT, MPI_COMM_WORLD);
        //std::cout << "Allgather 56 after" << std::endl;
    }

    // Gather the final result
    //std::cout << "before gather" << std::endl;
    MPI_Gather(localDist + startVertex, verticesPerProcess, MPI_INT, resDist, verticesPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    //std::cout << "gather 61 after" << std::endl;
}
