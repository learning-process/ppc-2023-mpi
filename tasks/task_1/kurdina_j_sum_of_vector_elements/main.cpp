// Copyright 2023 Kurdina Julia
#include <gtest/gtest.h>
#include <vector>
#include "./sum_elements.h"

TEST(Parallel_Operations_MPI, Test_all_zero) {
  int ProcRank;
  int columns = 4;
  int rows = 3;
  int l = 0;
  int r = 0;
  std::vector<int> M(columns * rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
  }
  int parallel_sum = get_sum_parallel(M, rows);
  if (ProcRank == 0) {
    int sequential_sum = get_sum(M);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_all_positive) {
  int ProcRank;
  int columns = 6;
  int rows = 11;
  int l = 1;
  int r = 100;
  std::vector<int> M(columns * rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
  }
  int parallel_sum = get_sum_parallel(M, rows);
  if (ProcRank == 0) {
    int sequential_sum = get_sum(M);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_all_negative) {
  int ProcRank;
  int columns = 6;
  int rows = 11;
  int l = -100;
  int r = -1;
  std::vector<int> M(columns * rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
  }
  int parallel_sum = get_sum_parallel(M, rows);
  if (ProcRank == 0) {
    int sequential_sum = get_sum(M);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Rand_Matrix_1) {
  int ProcRank;
  int columns = 15;
  int rows = 10;
  int l = -10;
  int r = 10;
  std::vector<int> M(columns * rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
  }
  int parallel_sum = get_sum_parallel(M, rows);
  if (ProcRank == 0) {
    int sequential_sum = get_sum(M);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Rand_Matrix_2) {
  int ProcRank;
  int columns = 30;
  int rows = 30;
  int l = -100;
  int r = 100;
  int result = 0;
  std::vector<int> M(columns * rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
  }
  int parallel_sum = get_sum_parallel(M, rows);
  if (ProcRank == 0) {
    int sequential_sum = get_sum(M);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}


int main(int argc, char** argv) {
  int res = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  res = RUN_ALL_TESTS();
  MPI_Finalize();

  return res;
}
