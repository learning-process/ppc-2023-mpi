// Copyright 2023 Shishkina Valeria
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./metodSimpsona.h"

TEST(MPI_TEST, Test1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double a = 0.0;
  const double b = 1.0;
  const double c = 0.0;
  const double d = 1.0;
  const double eps = 0.3;
  const int n = 100;

  double resParallel = parallelSimpson(a, b, c, d, n);
  if (rank == 0) {
    double res = simpson(a, b, c, d, n);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double a = 0.0;
  const double b = 0.0;
  const double c = 0.0;
  const double d = 0.0;
  const double eps = 0.3;
  const int n = 100;

  double resParallel = parallelSimpson(a, b, c, d, n);
  if (rank == 0) {
    double res = simpson(a, b, c, d, n);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double a = 1.0;
  const double b = 1.0;
  const double c = 1.0;
  const double d = 1.0;
  const double eps = 0.3;
  const int n = 100;

  double resParallel = parallelSimpson(a, b, c, d, n);
  if (rank == 0) {
    double res = simpson(a, b, c, d, n);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double a = 0.0;
  const double b = 2.0;
  const double c = 0.0;
  const double d = 2.0;
  const double eps = 0.3;
  const int n = 100;

  double resParallel = parallelSimpson(a, b, c, d, n);
  if (rank == 0) {
    double res = simpson(a, b, c, d, n);
    ASSERT_NEAR(resParallel, res, eps);
  }
}

TEST(MPI_TEST, Test5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const double a = 1.0;
  const double b = 2.0;
  const double c = 1.0;
  const double d = 2.0;
  const double eps = 0.3;
  const int n = 100;

  double resParallel = parallelSimpson(a, b, c, d, n);
  if (rank == 0) {
    double res = simpson(a, b, c, d, n);
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
