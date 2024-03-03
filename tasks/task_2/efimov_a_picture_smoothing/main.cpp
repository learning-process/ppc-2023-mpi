// Copyright 2024 Efimov Andrey
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./picture_smoothing.h"

TEST(Test_Picture_Smoothing, Test_image_three_for_four) {
  int row = 3, col = 4, smoothing = 60;
  std::vector <int> matr(row * col), seq_fin(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  matr = { 131, 86, 94, 200, 185, 140, 170, 200, 176, 147, 99, 151 };
  seq_fin = { 175, 247, 234, 64, 88, 160, 112, 64, 103, 149, 226, 143 };
  std::vector <int> fin(row * col);
  fin = par_smoothing(matr, row, col, smoothing);
  if (rank == 0) {
    ASSERT_EQ(seq_fin, fin);
  }
}

TEST(Test_Picture_Smoothing, Test_image_nine_for_fifteen) {
  int row = 9, col = 15, smoothing = 20;
  std::vector <int> matr(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matr = get_image(row, col);
  }
  std::vector <int> fin(row * col);
  fin = par_smoothing(matr, row, col, smoothing);
  std::vector <int> seq_fin(row * col);
  if (rank == 0) {
    seq_fin = seq_smoothing(matr, row, col, smoothing);
    ASSERT_EQ(seq_fin, fin);
  }
}

TEST(Test_Picture_Smoothing, Test_image_three_for_three) {
  int row = 3, col = 3, smoothing = 30;
  std::vector <int> matr(row * col), seq_fin(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  matr = { 100, 65, 79, 48, 54, 160, 73, 46, 91 };
  seq_fin = { 51, 97, 79, 119, 111, 0, 86, 121, 63 };
  std::vector <int> fin(row * col);
  fin = par_smoothing(matr, row, col, smoothing);
  if (rank == 0) {
    ASSERT_EQ(seq_fin, fin);
  }
}

TEST(Test_Picture_Smoothing, Test_image_small_smoothing) {
  int row = 5, col = 5, smoothing = 5;
  std::vector<int> matr(row * col), seq_fin(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  matr = { 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100,
  100, 100, 100, 100, 100,
  100, 100, 100, 100, 100,
  100, 100, 100, 100, 100 };

  seq_fin = { 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100,
  100, 100, 100, 100, 100,
  100, 100, 100, 100, 100,
  100, 100, 100, 100, 100 };

  std::vector<int> fin(row * col);
  fin = par_smoothing(matr, row, col, smoothing);
  if (rank == 0) {
  ASSERT_EQ(seq_fin, fin);
  }
}

TEST(Test_Picture_Smoothing, Test_image_1366x768) {
  int row = 1366, col = 768, smoothing = 80;
  std::vector <int> matr(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matr = get_image(row, col);
  }
  std::vector <int> fin(row * col);
  fin = par_smoothing(matr, row, col, smoothing);
  std::vector <int> seq_fin(row * col);
  if (rank == 0) {
    seq_fin = seq_smoothing(matr, row, col, smoothing);
    ASSERT_EQ(seq_fin, fin);
  }
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
