// Copyright 2023 Safronov Mikhail
#include <gtest/gtest.h>
#include "./dijkstra.h"


TEST(DijkstraMPI, ParallelTest_0) {
    int rank, size; const int V = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm test_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, 0, &test_comm);

    int** graph;
    int inputGraph[V][V] = {
        {0, 7, 1, 4, 37},
        {7, 0, 2, 5, 1},
        {1, 2, 0, 12, 3},
        {4, 5, 12, 0, 4},
        {2, 1, 3, 4, 0},
    };

    graph = new int*[V];
    for (int i = 0; i < V; ++i) {
        graph[i] = new int[V];
    }

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph[i][j] = inputGraph[i][j];
        }
    }

    MPI_Bcast(&(graph[0][0]), V * V, MPI_INT, 0, test_comm);

    MPI_Barrier(test_comm);

    int* resDist = new int[V];

    int source = 0;

    dijkstra(test_comm, graph, V, source, resDist);

    MPI_Barrier(test_comm);

    if (rank == 0) {
        int dist[V] = {0, 3, 1, 4, 4};

        for (int i = 0; i < V; i++) {
            ASSERT_EQ(resDist[i], dist[i]);
        }

        for (int i = 0; i < V; ++i) {
            delete[] graph[i];
        }
    }

    MPI_Comm_free(&test_comm);
    delete[] graph;
    delete[] resDist;
}


TEST(DijkstraMPI, ParallelTest_1) {
    int rank, size; const int V = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm test_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, 0, &test_comm);

    int** graph;
    int inputGraph[V][V] = {
        {0, 3, 2, 0, 0},
        {3, 0, 4, 8, 0},
        {2, 4, 0, 0, 0},
        {0, 8, 0, 0, 3},
        {0, 0, 0, 3, 0}
    };

    graph = new int*[V];
    for (int i = 0; i < V; ++i) {
        graph[i] = new int[V];
    }

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph[i][j] = inputGraph[i][j];
        }
    }

    MPI_Bcast(&(graph[0][0]), V * V, MPI_INT, 0, test_comm);

    MPI_Barrier(test_comm);

    int* resDist = new int[V];

    int source = 0;

    dijkstra(test_comm, graph, V, source, resDist);

    MPI_Barrier(test_comm);

    if (rank == 0) {
        int dist[V] = {0, 3, 2, 11, 14};

        for (int i = 0; i < V; i++) {
            ASSERT_EQ(resDist[i], dist[i]);
        }

        for (int i = 0; i < V; ++i) {
            delete[] graph[i];
        }
    }

    MPI_Comm_free(&test_comm);
    delete[] graph;
    delete[] resDist;
}

TEST(DijkstraMPI, ParallelTest_2) {
    int rank, size; const int V = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm test_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, 0, &test_comm);

    int** graph;
    int inputGraph[V][V] = {
        {0, 412, 51, 15, 73},
        {412, 0, 346, 1221, 125},
        {51, 346, 0, 164, 634},
        {15, 1221, 164, 0, 347},
        {73, 125, 634, 347, 0}
    };

    graph = new int*[V];
    for (int i = 0; i < V; ++i) {
        graph[i] = new int[V];
    }

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph[i][j] = inputGraph[i][j];
        }
    }

    MPI_Bcast(&(graph[0][0]), V * V, MPI_INT, 0, test_comm);

    MPI_Barrier(test_comm);

    int* resDist = new int[V];

    int source = 0;

    dijkstra(test_comm, graph, V, source, resDist);

    MPI_Barrier(test_comm);

    if (rank == 0) {
        int dist[V] = {0, 198, 51, 15, 73};

        for (int i = 0; i < V; i++) {
            ASSERT_EQ(resDist[i], dist[i]);
        }

        for (int i = 0; i < V; ++i) {
            delete[] graph[i];
        }
    }

    MPI_Comm_free(&test_comm);
    delete[] graph;
    delete[] resDist;
}

TEST(DijkstraMPI, ParallelTest_3) {
    int rank, size; const int V = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm test_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, 0, &test_comm);

    int** graph;
    int inputGraph[V][V] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    graph = new int*[V];
    for (int i = 0; i < V; ++i) {
        graph[i] = new int[V];
    }

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph[i][j] = inputGraph[i][j];
        }
    }

    MPI_Bcast(&(graph[0][0]), V * V, MPI_INT, 0, test_comm);

    MPI_Barrier(test_comm);

    int* resDist = new int[V];

    int source = 0;

    dijkstra(test_comm, graph, V, source, resDist);

    MPI_Barrier(test_comm);

    if (rank == 0) {
        int dist[V] = {0, INT_MAX, INT_MAX, INT_MAX, INT_MAX};

        for (int i = 0; i < V; i++) {
            ASSERT_EQ(resDist[i], dist[i]);
        }

        for (int i = 0; i < V; ++i) {
            delete[] graph[i];
        }
    }

    MPI_Comm_free(&test_comm);
    delete[] graph;
    delete[] resDist;
}

TEST(DijkstraMPI, ParallelTest_4) {
    int rank, size; const int V = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm test_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, 0, &test_comm);

    int** graph;
    int inputGraph[V][V] = {
        {0, 1, 4, 0, 0},
        {1, 0, 4, 2, 7},
        {4, 4, 0, 3, 5},
        {0, 2, 3, 0, 4},
        {0, 7, 5, 4, 0}
    };

    graph = new int*[V];
    for (int i = 0; i < V; ++i) {
        graph[i] = new int[V];
    }

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph[i][j] = inputGraph[i][j];
        }
    }

    MPI_Bcast(&(graph[0][0]), V * V, MPI_INT, 0, test_comm);

    MPI_Barrier(test_comm);

    int* resDist = new int[V];

    int source = 0;

    dijkstra(test_comm, graph, V, source, resDist);

    MPI_Barrier(test_comm);

    if (rank == 0) {
        int dist[V] = {0, 1, 4, 3, 7};

        for (int i = 0; i < V; i++) {
            ASSERT_EQ(resDist[i], dist[i]);
        }

        for (int i = 0; i < V; ++i) {
            delete[] graph[i];
        }
    }

    MPI_Comm_free(&test_comm);
    delete[] graph;
    delete[] resDist;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    ::testing::InitGoogleTest(&argc, argv);
    int resultCode = RUN_ALL_TESTS();

    MPI_Finalize();
    return resultCode;
}
