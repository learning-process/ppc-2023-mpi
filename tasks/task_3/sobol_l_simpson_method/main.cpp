// Copyright 2023 Sobol Lubov

#include <gtest/gtest.h>

#include <mpi.h>
#include <iostream>

#include "./simpson_method.h"

TEST(MPI_TEST, Test1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double start = 0.0;
  const double end = 1.0;
  const double lower = 0.0;
  const double upper = 1.0;
  const double eps = 0.3;
  const int intervals = 100;

  double resParallel = parallelSimpsonMethod(start, end, lower, upper, intervals);
  if (rank == 0) {
    double res = simpsonMethod(start, end, lower, upper, intervals);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double start = 0.0;
  const double end = 0.0;
  const double lower = 0.0;
  const double upper = 0.0;
  const double eps = 0.3;
  const int intervals = 100;

  double resParallel = parallelSimpsonMethod(start, end, lower, upper, intervals);
  if (rank == 0) {
    double res = simpsonMethod(start, end, lower, upper, intervals);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double start = 1.0;
  const double end = 1.0;
  const double lower = 1.0;
  const double upper = 1.0;
  const double eps = 0.3;
  const int intervals = 100;

  double resParallel = parallelSimpsonMethod(start, end, lower, upper, intervals);
  if (rank == 0) {
    double res = simpsonMethod(start, end, lower, upper, intervals);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double start = 0.0;
  const double end = 2.0;
  const double lower = 0.0;
  const double upper = 2.0;
  const double eps = 0.3;
  const int intervals = 100;

  double resParallel = parallelSimpsonMethod(start, end, lower, upper, intervals);
  if (rank == 0) {
    double res = simpsonMethod(start, end, lower, upper, intervals);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double start = 1.0;
  const double end = 2.0;
  const double lower = 1.0;
  const double upper = 2.0;
  const double eps = 0.3;
  const int intervals = 100;

  double resParallel = parallelSimpsonMethod(start, end, lower, upper, intervals);
  if (rank == 0) {
    double res = simpsonMethod(start, end, lower, upper, intervals);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int result_code = 0;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  if (proc_rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
