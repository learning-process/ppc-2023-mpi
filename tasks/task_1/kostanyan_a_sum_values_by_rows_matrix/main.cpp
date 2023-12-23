// Copyright 2023 Kostanyan Arsen
#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include "./alg.h"

TEST(Parallel_Operations_MPI, Matrix_4x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int rows = 4, cols = 4;

    if (rank == 0) {
        std::vector<int> tempMatrix = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        matrix = tempMatrix;
    }

    std::vector<int> global_sum = ParallelProcessing(matrix, rows, cols);

    if (rank == 0) {
        std::vector<int> reference_sum = calculateRowSums(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_5x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int rows = 5, cols = 5;

    if (rank == 0) {
        matrix = RandomMatrix(rows, cols);
    }

    std::vector<int> global_sum = ParallelProcessing(matrix, rows, cols);

    if (rank == 0) {
        std::vector<int> reference_sum = calculateRowSums(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_5x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int rows = 5, cols = 10;

    if (rank == 0) {
        matrix = RandomMatrix(rows, cols);
    }

    std::vector<int> global_sum = ParallelProcessing(matrix, rows, cols);

    if (rank == 0) {
        std::vector<int> reference_sum = calculateRowSums(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_10x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int rows = 10, cols = 5;

    if (rank == 0) {
        matrix = RandomMatrix(rows, cols);
    }

    std::vector<int> global_sum = ParallelProcessing(matrix, rows, cols);

    if (rank == 0) {
        std::vector<int> reference_sum = calculateRowSums(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_100x200) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int rows = 100, cols = 200;

    if (rank == 0) {
        matrix = RandomMatrix(rows, cols);
    }

    std::vector<int> global_sum = ParallelProcessing(matrix, rows, cols);

    if (rank == 0) {
        std::vector<int> reference_sum = calculateRowSums(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
