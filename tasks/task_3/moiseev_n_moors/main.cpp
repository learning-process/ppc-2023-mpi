// Copyright 2023 by Nikita Moiseev
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "task_3/moiseev_n_moors/moors.h"

TEST(MPI_TESTS, Exception_Thrown_If_Incorrect_Source_Seq) {
    int process_rank;
    int graph_size = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    std::vector<int> graph(graph_size * graph_size);
    if (process_rank == 0)
        graph = generateRandomWeightedGraph(graph_size);
    std::vector<int> result(graph_size);
    if (process_rank == 0) {
        ASSERT_ANY_THROW(sequentialMooreAlgorithm(graph, (-1)));
    }
}

TEST(MPI_TESTS, No_Exception_Thrown_If_Correct_Source_Seq) {
    int process_rank;
    int graph_size = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    std::vector<int> graph(graph_size * graph_size);
    if (process_rank == 0)
        graph = generateRandomWeightedGraph(graph_size);
    std::vector<int> result(graph_size);
    if (process_rank == 0) {
        ASSERT_NO_THROW(sequentialMooreAlgorithm(graph, (1)));
    }
}

TEST(MPI_TESTS, Test_On_Size_10) {
    int process_rank;
    int graph_size = 10;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    std::vector<int> graph(graph_size * graph_size);
    if (process_rank == 0)
        graph = generateRandomWeightedGraph(graph_size);
    std::vector<int> parallel_result(graph_size);
    std::vector<int> sequential_result(graph_size);
    parallel_result = parallelMooreAlgorithm(graph, 0);
    if (process_rank == 0) {
        sequential_result = sequentialMooreAlgorithm(graph, 0);
        for (int i = 0; i < graph_size; ++i) {
            ASSERT_EQ(sequential_result[i], parallel_result[i]);
        }
    }
}

TEST(MPI_TESTS, Test_On_Size_100) {
    int process_rank;
    int graph_size = 100;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    std::vector<int> graph(graph_size * graph_size);
    if (process_rank == 0)
        graph = generateRandomWeightedGraph(graph_size);
    std::vector<int> parallel_result(graph_size);
    std::vector<int> sequential_result(graph_size);
    parallel_result = parallelMooreAlgorithm(graph, 0);
    if (process_rank == 0) {
        sequential_result = sequentialMooreAlgorithm(graph, 0);
        for (int i = 0; i < graph_size; ++i) {
            ASSERT_EQ(sequential_result[i], parallel_result[i]);
        }
    }
}

TEST(MPI_TESTS, Test_On_Size_1) {
    int process_rank;
    int graph_size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    std::vector<int> graph(graph_size * graph_size);
    if (process_rank == 0)
        graph = generateRandomWeightedGraph(graph_size);
    std::vector<int> parallel_result(graph_size);
    std::vector<int> sequential_result(graph_size);
    parallel_result = parallelMooreAlgorithm(graph, 0);
    if (process_rank == 0) {
        sequential_result = sequentialMooreAlgorithm(graph, 0);
        for (int i = 0; i < graph_size; ++i) {
            ASSERT_EQ(sequential_result[i], parallel_result[i]);
        }
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
