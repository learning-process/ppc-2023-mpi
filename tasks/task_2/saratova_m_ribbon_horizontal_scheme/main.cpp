// Copyright 2023 Saratova Marina
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./ribbon_horizontal_scheme.h"

TEST(MPI_TESTS, Test_random_matrix) {
  int rank = 0;
  int size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::vector<int>> matrix1 = generate_random_matrix
  (30, 30, 1, 10);
  std::vector<std::vector<int>> matrix2 = generate_random_matrix
  (30, 30, 1, 10);

  std::vector<std::vector<int>> par_result = par_ribbon_horizontal_scheme
  (matrix1, matrix2);

  if (rank == 0) {
    std::vector<std::vector<int>> seq_result = seq_matrix_multiply
    (matrix1, matrix2);
    ASSERT_EQ(true, compare_matrix(seq_result, par_result));
  }
}

TEST(MPI_TESTS, Test_given_matrix) {
  int rank = 0;
  int size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::vector<int>> matrix1 = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12},
    {13, 14, 15, 16, 17, 18},
    {19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 29, 30},
    {31, 32, 33, 34, 35, 36}
  };
  std::vector<std::vector<int>> matrix2 = {
    {5, 10, 15, 20, 25, 30},
    {4, 9, 14, 19, 24, 29},
    {3, 8, 13, 18, 23, 28},
    {2, 7, 12, 17, 22, 27},
    {1, 6, 11, 16, 21, 26},
    {0, -5, -10, -15, -20, -25}
  };

  std::vector<std::vector<int>> par_result = par_ribbon_horizontal_scheme
  (matrix1, matrix2);

  if (rank == 0) {
    std::vector<std::vector<int>> seq_result = seq_matrix_multiply
    (matrix1, matrix2);
    ASSERT_EQ(true, compare_matrix(seq_result, par_result));
  }
}

TEST(MPI_TESTS, Test_big_matrix) {
  int rank = 0;
  int size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::vector<int>> matrix1 = generate_random_matrix
  (100, 100, 1, 10);
  std::vector<std::vector<int>> matrix2 = generate_random_matrix
  (100, 100, 1, 10);

  std::vector<std::vector<int>> par_result = par_ribbon_horizontal_scheme
  (matrix1, matrix2);

  if (rank == 0) {
    std::vector<std::vector<int>> seq_result = seq_matrix_multiply
    (matrix1, matrix2);
    ASSERT_EQ(true, compare_matrix(seq_result, par_result));
  }
}

TEST(MPI_TESTS, Test_matrix_with_big_values) {
  int rank = 0;
  int size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::vector<int>> matrix1 = generate_random_matrix
  (30, 30, 1, 10000000);
  std::vector<std::vector<int>> matrix2 = generate_random_matrix
  (30, 30, 1, 10000000);

  std::vector<std::vector<int>> par_result = par_ribbon_horizontal_scheme
  (matrix1, matrix2);

  if (rank == 0) {
    std::vector<std::vector<int>> seq_result = seq_matrix_multiply
    (matrix1, matrix2);
    ASSERT_EQ(true, compare_matrix(seq_result, par_result));
  }
}

TEST(MPI_TESTS, Test_matrix_with_negative_values) {
  int rank = 0;
  int size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::vector<int>> matrix1 = generate_random_matrix
  (30, 30, -10000000, 0);
  std::vector<std::vector<int>> matrix2 = generate_random_matrix
  (30, 30, -10000000, 0);

  std::vector<std::vector<int>> par_result = par_ribbon_horizontal_scheme
  (matrix1, matrix2);

  if (rank == 0) {
    std::vector<std::vector<int>> seq_result = seq_matrix_multiply
    (matrix1, matrix2);
    ASSERT_EQ(true, compare_matrix(seq_result, par_result));
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
