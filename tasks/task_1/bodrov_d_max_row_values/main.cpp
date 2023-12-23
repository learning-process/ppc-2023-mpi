// Copyright 2023 Bodrov Daniil
#include <gtest/gtest.h>
#include <vector>
#include "./max_row_values.h"

TEST(Parallel_Operations_MPI, Test_FindMaxRowValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = 3;
    std::vector<int> max_values;
    if (rank == 0) {
        max_values = FindMaxRowValues(matrix, n);
    }
    std::vector<int> parallel_max_values = FindMaxRowValuesPar(matrix, n);
    if (rank == 0) {
        ASSERT_EQ(max_values, parallel_max_values);
    }
}

TEST(Parallel_Operations_MPI, Test_FindMaxRowValues_Single_Row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = {1, 2, 3, 4, 5};
    int n = 1;
    std::vector<int> max_values;
    if (rank == 0) {
        max_values = FindMaxRowValues(matrix, n);
    }
    std::vector<int> parallel_max_values = FindMaxRowValuesPar(matrix, n);
    if (rank == 0) {
        ASSERT_EQ(max_values, parallel_max_values);
    }
}

TEST(Parallel_Operations_MPI, Test_FindMaxRowValues_Same_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = {5, 5, 5, 5, 5, 5, 5, 5, 5};
    int n = 3;
    std::vector<int> max_values;
    if (rank == 0) {
        max_values = FindMaxRowValues(matrix, n);
    }
    std::vector<int> parallel_max_values = FindMaxRowValuesPar(matrix, n);
    if (rank == 0) {
        ASSERT_EQ(max_values, parallel_max_values);
    }
}

TEST(Parallel_Operations_MPI, Test_FindMaxRowValues_Negative_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = {-1, -2, -3, -4, -5, -6, -7, -8, -9};
    int n = 3;
    std::vector<int> max_values;
    if (rank == 0) {
        max_values = FindMaxRowValues(matrix, n);
    }
    std::vector<int> parallel_max_values = FindMaxRowValuesPar(matrix, n);
    if (rank == 0) {
        ASSERT_EQ(max_values, parallel_max_values);
    }
}

TEST(Parallel_Operations_MPI, Test_FindMaxRowValues_Empty_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int n = 0;
    std::vector<int> max_values;
    if (rank == 0) {
        max_values = FindMaxRowValues(matrix, n);
    }
    std::vector<int> parallel_max_values = FindMaxRowValuesPar(matrix, n);
    if (rank == 0) {
        ASSERT_EQ(max_values, parallel_max_values);
    }
}

TEST(Parallel_Operations_MPI, Test_FindMaxRowValues_Large_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix(10000, 1);
    int n = 100;
    std::vector<int> max_values;
    if (rank == 0) {
        max_values = FindMaxRowValues(matrix, n);
    }
    std::vector<int> parallel_max_values = FindMaxRowValuesPar(matrix, n);
    if (rank == 0) {
        ASSERT_EQ(max_values, parallel_max_values);
    }
}


int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
