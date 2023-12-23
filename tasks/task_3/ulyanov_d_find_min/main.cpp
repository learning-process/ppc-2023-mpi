// Copyright 2023 Ulyanov Daniil
#include <gtest/gtest.h>
#include <mpi.h>
#include "task_3/ulyanov_d_find_min/find_min.h"

TEST(find_min, test1) {
  int rank, count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  auto func = [](double x) { return std::sin(x); };
  double resPar = findMinPar(-3.14 / 2, 3.14 / 2, func, 0.001);
  double resSeq = findMinSeq(-3.14 / 2, 3.14 / 2, func, 0.001);

  if (rank == 0) {
    ASSERT_NEAR(-1, resPar, 0.001);
    ASSERT_NEAR(-1, resSeq, 0.001);
  }
}

TEST(find_min, test2) {
  int rank, count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  auto func = [](double x) { return std::cos(x); };
  double resPar = findMinPar(-3.14, 3.14, func, 0.001);
  double resSeq = findMinSeq(-3.14, 3.14, func, 0.001);

  if (rank == 0) {
    ASSERT_NEAR(-1, resPar, 0.001);
    ASSERT_NEAR(-1, resSeq, 0.001);
  }
}

TEST(find_min, test3) {
  int rank, count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  auto func = [](double x) { return x; };
  double resPar = findMinPar(-3, 3, func, 0.0001);
  double resSeq = findMinSeq(-3, 3, func, 0.0001);

  if (rank == 0) {
    ASSERT_NEAR(-3, resPar, 0.01);
    ASSERT_NEAR(-3, resSeq, 0.01);
  }
}

TEST(find_min, test4) {
  int rank, count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  auto func = [](double x) { return sqrt(x); };
  double resPar = findMinPar(1, 4, func, 0.00001);
  double resSeq = findMinSeq(1, 4, func, 0.00001);

  if (rank == 0) {
    ASSERT_NEAR(1, resPar, 0.001);
    ASSERT_NEAR(1, resSeq, 0.001);
  }
}

TEST(find_min, test5) {
  int rank, count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  auto func = [](double x) { return pow(2, x); };
  double resPar = findMinPar(2, 4, func, 0.00001);
  double resSeq = findMinSeq(2, 4, func, 0.00001);

  if (rank == 0) {
    ASSERT_NEAR(4, resPar, 0.001);
    ASSERT_NEAR(4, resSeq, 0.001);
  }
}


int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
