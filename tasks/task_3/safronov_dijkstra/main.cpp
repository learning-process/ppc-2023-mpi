// Copyright 2023 Safronov Mikhail
#include <gtest/gtest.h>
#include "./dijkstra.h"


TEST(DijkstraMPI, ParallelTest_0) {
    int rank, size, V = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //std::cout << rank << " - " << size << std::endl;

    int dist[V] = {0, 3, 2, 11, 14};
    int* resDist = new int[V];
    
    int** graph = new int*[V];
    for (int i = 0; i < V; ++i) {
        graph[i] = new int[V];
    }

    int inputGraph[V][V] = {
        {0, 3, 2, 0, 0},
        {3, 0, 4, 8, 0},
        {2, 4, 0, 0, 0},
        {0, 8, 0, 0, 3},
        {0, 0, 0, 3, 0}
    };

    // Copy values to dynamic array
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph[i][j] = inputGraph[i][j];
        }
    }

    // Source vertex
    int source = 0;
    MPI_Barrier(MPI_COMM_WORLD);

    dijkstra(graph, V, source, resDist);

    // If you want to synchronize MPI processes, add a barrier
    MPI_Barrier(MPI_COMM_WORLD);

    for(int i=0;i < V; i++) {
        std::cout << resDist[i] << std::endl;
    }

    // Perform checks based on the MPI rank
    if (rank == 0) {
        int* gatheredDist = new int[V * size];
        //std::cout << "before gather" << std::endl;
        MPI_Gather(resDist, V, MPI_INT, gatheredDist, V, MPI_INT, 0, MPI_COMM_WORLD);
        //std::cout << "gather 54 after" << std::endl;

        for (int i = 0; i < V * size; i++) {
            ASSERT_EQ(gatheredDist[i], dist[i]);
        }

        delete[] gatheredDist;
    }

    for (int i = 0; i < V; ++i) {
        delete[] graph[i];
    }
    delete[] graph;
    delete[] resDist;
}



TEST(DijkstraMPI, ParallelTest_1) {
    // int rank, size,  V = 5;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);  // Add this line

    // std::cout << rank << std::endl;

    // int dist[V] = {0, 1, 4, 3, 7};
    // int* resDist = new int[V];

    // int** graph = new int*[V];
    // for (int i = 0; i < V; ++i) {
    //     graph[i] = new int[V];
    // }

    // int inputGraph[V][V] = {
    //         {0, 1, 4, 0, 0},
    //         {1, 0, 4, 2, 7},
    //         {4, 4, 0, 3, 5},
    //         {0, 2, 3, 0, 4},
    //         {0, 7, 5, 4, 0}
    // };

    // // Copy values to dynamic array
    // for (int i = 0; i < V; ++i) {
    //     for (int j = 0; j < V; ++j) {
    //         graph[i][j] = inputGraph[i][j];
    //     }
    // }

    // // Source vertex
    // int source = 0;
    // resDist = dijkstra(graph, V, source);

    // if (rank == 0) {
    //     // Only perform this check on the root process
    //     int* gatheredDist = new int[V * size];
    //     MPI_Gather(dist, V, MPI_INT, gatheredDist, V, MPI_INT, 0, MPI_COMM_WORLD);

    //     for (int i = 0; i < V; i++) {
    //         //std::cout << "dist " << gatheredDist[i] << " resDist " << resDist[i] << std::endl;
    //         ASSERT_EQ(gatheredDist[i], resDist[i]);
    //     }

    //     delete[] gatheredDist;
    // }

    // for (int i = 0; i < V; ++i) {
    //     delete[] graph[i];
    // }
    // delete[] graph;
    // delete[] resDist;
}


TEST(DijkstraMPI, ParallelTest_2) {
    // int rank, size,  V = 5;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);  // Add this line

    // std::cout << rank << std::endl;

    // int dist[V] = {0, 1, 4, 3, 7};
    // int* resDist = new int[V];

    // int** graph = new int*[V];
    // for (int i = 0; i < V; ++i) {
    //     graph[i] = new int[V];
    // }

    // int inputGraph[V][V] = {
    //         {0, 1, 4, 0, 0},
    //         {1, 0, 4, 2, 7},
    //         {4, 4, 0, 3, 5},
    //         {0, 2, 3, 0, 4},
    //         {0, 7, 5, 4, 0}
    // };

    // // Copy values to dynamic array
    // for (int i = 0; i < V; ++i) {
    //     for (int j = 0; j < V; ++j) {
    //         graph[i][j] = inputGraph[i][j];
    //     }
    // }

    // // Source vertex
    // int source = 0;
    // resDist = dijkstra(graph, V, source);

    // if (rank == 0) {
    //     // Only perform this check on the root process
    //     int* gatheredDist = new int[V * size];
    //     MPI_Gather(dist, V, MPI_INT, gatheredDist, V, MPI_INT, 0, MPI_COMM_WORLD);

    //     for (int i = 0; i < V; i++) {
    //         //std::cout << "dist " << gatheredDist[i] << " resDist " << resDist[i] << std::endl;
    //         ASSERT_EQ(gatheredDist[i], resDist[i]);
    //     }

    //     delete[] gatheredDist;
    // }

    // for (int i = 0; i < V; ++i) {
    //     delete[] graph[i];
    // }
    // delete[] graph;
    // delete[] resDist;
}

TEST(DijkstraMPI, ParallelTest_3) {
    // int rank, size,  V = 5;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);  // Add this line

    // std::cout << rank << std::endl;

    // int dist[V] = {0, 99999999, 99999999, 99999999, 99999999};
    // int* resDist = new int[V];

    // int** graph = new int*[V];
    // for (int i = 0; i < V; ++i) {
    //     graph[i] = new int[V];
    // }

    // int inputGraph[V][V] = {
    //     {0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0}
    // };

    // // Copy values to dynamic array
    // for (int i = 0; i < V; ++i) {
    //     for (int j = 0; j < V; ++j) {
    //         graph[i][j] = inputGraph[i][j];
    //     }
    // }

    // // Source vertex
    // int source = 0;
    // resDist = dijkstra(graph, V, source);

    // if (rank == 0) {
    //     // Only perform this check on the root process
    //     int* gatheredDist = new int[V * size];
    //     MPI_Gather(dist, V, MPI_INT, gatheredDist, V, MPI_INT, 0, MPI_COMM_WORLD);

    //     for (int i = 0; i < V; i++) {
    //         //std::cout << "dist " << gatheredDist[i] << " resDist " << resDist[i] << std::endl;
    //         ASSERT_EQ(gatheredDist[i], resDist[i]);
    //     }

    //     delete[] gatheredDist;
    // }

    // for (int i = 0; i < V; ++i) {
    //     delete[] graph[i];
    // }
    // delete[] graph;
    // delete[] resDist;
}

TEST(DijkstraMPI, ParallelTest_4) {
    // int rank, size,  V = 5;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);  // Add this line

    // std::cout << rank << std::endl;

    // int dist[V] = {0, 198, 51, 15, 73};
    // int* resDist = new int[V];

    // int** graph = new int*[V];
    // for (int i = 0; i < V; ++i) {
    //     graph[i] = new int[V];
    // }

    // int inputGraph[V][V] = {
    //     {0, 412, 51, 15, 73},
    //     {412, 0, 346, 1221, 125},
    //     {51, 346, 0, 164, 634},
    //     {15, 1221, 164, 0, 347},
    //     {73, 125, 634, 347, 0}
    // };

    // // Copy values to dynamic array
    // for (int i = 0; i < V; ++i) {
    //     for (int j = 0; j < V; ++j) {
    //         graph[i][j] = inputGraph[i][j];
    //     }
    // }

    // // Source vertex
    // int source = 0;
    // resDist = dijkstra(graph, V, source);

    // if (rank == 0) {
    //     // Only perform this check on the root process
    //     int* gatheredDist = new int[V * size];
    //     MPI_Gather(dist, V, MPI_INT, gatheredDist, V, MPI_INT, 0, MPI_COMM_WORLD);

    //     for (int i = 0; i < V; i++) {
    //         //std::cout << "dist " << gatheredDist[i] << " resDist " << resDist[i] << std::endl;
    //         ASSERT_EQ(gatheredDist[i], resDist[i]);
    //     }

    //     delete[] gatheredDist;
    // }

    // for (int i = 0; i < V; ++i) {
    //     delete[] graph[i];
    // }
    // delete[] graph;
    // delete[] resDist;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ::testing::InitGoogleTest(&argc, argv);
    int resultCode = RUN_ALL_TESTS();

    MPI_Finalize();
    return resultCode;
}


