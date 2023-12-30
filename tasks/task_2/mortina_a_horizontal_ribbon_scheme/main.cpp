// Copyright 2023 Mortina Anastasia

#include <mpi.h>
#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include "./horizontal_scheme.h"


TEST(Parallel_Vector_Sum, Test_with_elems) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 3;
    int cols = 3;

    std::vector<int> matrix{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> vec{1, 2, 3};

    std::vector<int> result(vec.size(), 0);
    result = ParallMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq = {14, 32, 50};
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_with_zero_matrix) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 4;
    int cols = 4;

    std::vector<int> matrix{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> vec{10, 24, 33, 14};

    std::vector<int> result(vec.size(), 0);
    result = ParallMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq = {0, 0, 0, 0};
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_with_empty) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> matrix{};
    std::vector<int> vec{};

    std::vector<int> result(vec.size(), 0);
    result = ParallMatrixVector(matrix, vec, 0, 0);

    if (rank == 0) {
        std::vector<int> resSeq{};
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_with) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 3;
    int cols = 7;

    std::vector<int> matrix = {12, 12, 56, 12, 64, 12, 74,
                                6, 2, 1, 5, 2, 5, 8,
                                4, 2, 12, 3, 4, 1, 12};
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7};


    std::vector<int> result(vec.size());
    result = ParallMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq{1162, 129, 166};
        ASSERT_EQ(result, resSeq);
    }
}


TEST(Parallel_Vector_Sum, Test_w4) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 10;
    int cols = 10;

    std::vector<int> matrix = generateMatrixRandom(rows, cols, -100, 100);

    std::vector<int> vec(rows);
    for (auto& elem : vec) generateRandomNumbers(-100, 100);

    int rowsPerProcess = rows / size;
    int start = rank * rowsPerProcess;
    int end = (rank == size - 1) ? rows : (rank + 1) * rowsPerProcess;

    std::vector<int> result(vec.size());
    result = ParallMatrixVector(matrix, vec, rows, cols);

    if (rank == 0) {
        std::vector<int> resSeq(10);
        matrixVectorMultiply(matrix, vec, &resSeq, cols, start, end);
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
