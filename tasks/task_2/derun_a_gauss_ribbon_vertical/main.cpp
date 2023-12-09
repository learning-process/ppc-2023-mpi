// Copyright 2023 Derun Andrew
#include <gtest/gtest.h>  // 123
#include <mpi.h>

#include <iostream>

#include "task_2/derun_a_gauss_ribbon_vertical/gauss_ribbon_vertical.h"

TEST(Gauss_Ribbon_Vertical_MPI, test_1_negative_size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int h = -3;
  const int w = -4;

  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomMatrix(w, h));
  }
}

TEST(Gauss_Ribbon_Vertical_MPI, test_2_negative_size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int h = -3;
  const int w = -4;

  if (rank == 0) {
    double matrix[12]{1, 2, -3, -4, 2, 5, -4, 0, -3, 1, 3, 5};
    ASSERT_ANY_THROW(getSequentialOperations(matrix, w, h));
  }
}

TEST(Gauss_Ribbon_Vertical_MPI, test_3_negative_size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int h = -3;
  const int w = -4;
  double matrix[12]{1, 2, -3, -4, 2, 5, -4, 0, -3, 1, 3, 5};

  ASSERT_ANY_THROW(getParallelOperations(matrix, w, h));
}

TEST(Gauss_Ribbon_Vertical_MPI, test_4_sequential) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double* res = 0;
  const int h = 3;
  const int w = 4;

  if (rank == 0) {
    double matrix[h * w]{1, 2, -3, -4, 2, 5, -4, 0, -3, 1, 3, 5};
    double x[h]{2.05, 1.7, 3.15};
    res = getSequentialOperations(matrix, w, h);
    for (int i = 0; i < h; i++) ASSERT_NEAR(x[i], res[i], 0.000001);
  }
}

TEST(Gauss_Ribbon_Vertical_MPI, test_5_parallel) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double* res = 0;
  const int h = 3;
  const int w = 4;
  double matrix[h * w]{1, 2, -3, -4, 2, 5, -4, 0, -3, 1, 3, 5};
  double x[h]{2.05, 1.7, 3.15};

  res = getParallelOperations(matrix, w, h);
  if (rank == 0) {
    for (int i = 0; i < h; i++) ASSERT_NEAR(x[i], res[i], 0.000001);
  }
}

TEST(Gauss_Ribbon_Vertical_MPI, test_6_sequential_and_parallel) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double* res = 0;
  const int h = 3;
  const int w = 4;
  double matrix[h * w]{1, 2, -3, -4, 2, 5, -4, 0, -3, 1, 3, 5};
  double* x = 0;

  res = getParallelOperations(matrix, w, h);
  if (rank == 0) {
    x = getSequentialOperations(matrix, w, h);
    for (int i = 0; i < h; i++) {
      ASSERT_NEAR(x[i], res[i], 0.000001);
    }
  }
}

TEST(Gauss_Ribbon_Vertical_MPI, test_7_sequential_and_parallel_other_values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double* res = 0;
  const int h = 3;
  const int w = 4;
  double matrix[h * w]{5, -2, 4, 5, 2, 3, -1, 7, 3, -1, 2, 3};
  double* x = 0;

  res = getParallelOperations(matrix, w, h);
  if (rank == 0) {
    x = getSequentialOperations(matrix, w, h);
    for (int i = 0; i < h; i++) {
      ASSERT_NEAR(x[i], res[i], 0.000001);
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
