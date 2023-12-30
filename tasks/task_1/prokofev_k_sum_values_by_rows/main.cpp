// Copyright 2023 Prokofev Kirill
#include <gtest/gtest.h>
#include <mpi.h>
#include<iostream>
#include "./sum_values_by_rows.h"

TEST(Parallel_Sum_Values_By_Rows_Matrix, Matrix_2x2) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> matrix;
  int n = 2;
  int m = 2;
  if (rankProc == 0) {
    matrix = {4, 5, 6, 7};
  }
  std::vector<int> parallSum = ParallSumValuesByRows(matrix, n, m);
  if (rankProc == 0) {
    std::vector<int> refSum = SeqSumValuesByRows(matrix, n, m);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(refSum[i], parallSum[i]);
    }
  }
}

TEST(Parallel_Sum_Values_By_Rows_Matrix, RandomMatrix_3x3) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> matrix;
  int n = 3;
  int m = 3;
  if (rankProc == 0) {
    matrix = GenerateRandomMatrix(n, m);
  }
  std::vector<int> parallSum = ParallSumValuesByRows(matrix, n, m);
  if (rankProc == 0) {
    std::vector<int> refSum = SeqSumValuesByRows(matrix, n, m);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(refSum[i], parallSum[i]);
    }
  }
}


TEST(Parallel_Sum_Values_By_Rows_Matrix, RandomMatrix_3x4) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> matrix;
  int n = 3;
  int m = 4;
  if (rankProc == 0) {
    matrix = GenerateRandomMatrix(n, m);
  }
  std::vector<int> parallSum = ParallSumValuesByRows(matrix, n, m);
  if (rankProc == 0) {
    std::vector<int> refSum = SeqSumValuesByRows(matrix, n, m);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(refSum[i], parallSum[i]);
    }
  }
}

TEST(Parallel_Sum_Values_By_Rows_Matrix, RandomMatrix_100x100) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> matrix;
  int n = 100;
  int m = 100;
  if (rankProc == 0) {
    matrix = GenerateRandomMatrix(n, m);
  }
  std::vector<int> parallSum = ParallSumValuesByRows(matrix, n, m);
  if (rankProc == 0) {
    std::vector<int> refSum = SeqSumValuesByRows(matrix, n, m);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(refSum[i], parallSum[i]);
    }
  }
}
TEST(Parallel_Sum_Values_By_Rows_Matrix, RandomMatrix_200x2) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> matrix;
  int n = 200;
  int m = 2;
  if (rankProc == 0) {
    matrix = GenerateRandomMatrix(n, m);
  }
  std::vector<int> parallSum = ParallSumValuesByRows(matrix, n, m);
  if (rankProc == 0) {
    std::vector<int> refSum = SeqSumValuesByRows(matrix, n, m);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(refSum[i], parallSum[i]);
    }
  }
}

int main(int argc, char** argv) {
  int result_code = 0;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();
  return result_code;
}
