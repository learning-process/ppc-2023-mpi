// Copyright 2023 Sobol Lubov

#include <mpi.h>
#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include "./horizontal_scheme.h"


TEST(Parallel_Vector_Sum, Test_Base) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 2;
    int cols = 2;

    std::vector<int> matrix{1, 1, 1, 1};
    std::vector<int> vec{1, 2};

    std::vector<int> result(vec.size(), 0);
    result = parallelMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq = {3, 3};
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_Zero_Matrix) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 2;
    int cols = 2;

    std::vector<int> matrix{0, 0, 0, 0};
    std::vector<int> vec{1, 2};

    std::vector<int> result(vec.size(), 0);
    result = parallelMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq = {0, 0};
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_Empty) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> matrix{};
    std::vector<int> vec{};

    std::vector<int> result(vec.size(), 0);
    result = parallelMatrixVector(matrix, vec, 0, 0);

    if (rank == 0) {
        std::vector<int> resSeq{};
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_Rectangle_Matrix) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 2;
    int cols = 3;

    std::vector<int> matrix = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    std::vector<int> vec{1, 2, 3};


    std::vector<int> result(vec.size());
    result = parallelMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq{6, 12, 18};
        ASSERT_EQ(result, resSeq);
    }
}


TEST(Parallel_Vector_Sum, Test_Random) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 2;
    int cols = 2;

    std::vector<int> matrix = generateRandomMatrix(rows, cols, -5, 5);

    std::vector<int> vec(rows);
    for (auto& elem : vec) generateSingleRandomNumber(-5, 5);

    int rowsPerProcess = rows / size;
    int start = rank * rowsPerProcess;
    int end = (rank == size - 1) ? rows : (rank + 1) * rowsPerProcess;

    std::vector<int> result(vec.size());
    result = parallelMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq(10);
        multiplyMatrixVector(matrix, vec, &resSeq, cols, start, end);
        ASSERT_EQ(result, resSeq);
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
