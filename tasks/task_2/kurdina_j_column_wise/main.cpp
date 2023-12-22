// Copyright 2023 Kurdina Julia
#include <gtest/gtest.h>
#include <vector>
#include "./column_wise.h"

TEST(Parallel_Operations_MPI, Test_all_zero) {
  int ProcRank;
  int columns = 4;
  int rows = 3;
  int l = 0;
  int r = 0;
  std::vector<int> M(columns * rows);
  std::vector<int> b(columns);
  std::vector<int> c(rows);
  std::vector<int> c_sequential(rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
    b = get_rand_matrix(1, columns, l, r);
  }
  c = columnwise_sum_parallel(M, b, rows, columns);
  if (ProcRank == 0) {
    c_sequential = columnwise_sum(M, b, rows, columns);
    ASSERT_EQ(c_sequential, c);
  }
}

TEST(Parallel_Operations_MPI, Test_all_positive) {
  int ProcRank;
  int columns = 6;
  int rows = 11;
  int l = 1;
  int r = 100;
  std::vector<int> M(columns * rows);
  std::vector<int> b(columns);
  std::vector<int> c(rows);
  std::vector<int> c_sequential(rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
    b = get_rand_matrix(1, columns, l, r);
  }
  c = columnwise_sum_parallel(M, b, rows, columns);
  if (ProcRank == 0) {
    c_sequential = columnwise_sum(M, b, rows, columns);
    ASSERT_EQ(c_sequential, c);
  }
}

TEST(Parallel_Operations_MPI, Test_all_negative) {
  int ProcRank;
  int columns = 6;
  int rows = 11;
  int l = -100;
  int r = -1;
  std::vector<int> M(columns * rows);
  std::vector<int> b(columns);
  std::vector<int> c(rows);
  std::vector<int> c_sequential(rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
    b = get_rand_matrix(1, columns, l, r);
  }
  c = columnwise_sum_parallel(M, b, rows, columns);
  if (ProcRank == 0) {
    c_sequential = columnwise_sum(M, b, rows, columns);
    ASSERT_EQ(c_sequential, c);
  }
}

TEST(Parallel_Operations_MPI, Test_Rand_Matrix_1) {
  int ProcRank;
  int columns = 15;
  int rows = 10;
  int l = -10;
  int r = 10;
  std::vector<int> M(columns * rows);
  std::vector<int> b(columns);
  std::vector<int> c(rows);
  std::vector<int> c_sequential(rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
    b = get_rand_matrix(1, columns, l, r);
  }
  c = columnwise_sum_parallel(M, b, rows, columns);
  if (ProcRank == 0) {
    c_sequential = columnwise_sum(M, b, rows, columns);
    ASSERT_EQ(c_sequential, c);
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
  std::vector<int> b(columns);
  std::vector<int> c(rows);
  std::vector<int> c_sequential(rows);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    M = get_rand_matrix(columns, rows, l, r);
    b = get_rand_matrix(1, columns, l, r);
  }
  c = columnwise_sum_parallel(M, b, rows, columns);
  if (ProcRank == 0) {
    c_sequential = columnwise_sum(M, b, rows, columns);
    ASSERT_EQ(c_sequential, c);
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
