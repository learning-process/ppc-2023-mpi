// Copyright 2023 Shubin Mikhail
#include <gtest/gtest.h>
#include "./min_values_columns_matrix.h"

TEST(Min_Values_Columns_Matrix, Test_min_given_matrix) {
  const int ROW = 3,
            COL = 4;
  std::vector<int> matr = { 5, 6, 7, 8, -1, -2, -3, -4, 9, 10, 11, 12 },
    min_v_par,
    min_v_seq;
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  min_v_par = getColumnMin_par(matr, ROW, COL);
  if (rank == 0) {
    min_v_seq = getColumnMin_seq(matr, ROW, COL);
    ASSERT_EQ(min_v_par, min_v_seq);
  }
}

TEST(Min_Values_Columns_Matrix, Test_min_random_matrix) {
  const int ROW = 3,
            COL = 4,
            MIN = -100,
            MAX = 100;
  std::vector<int> matr = getRandomMatrix(ROW, COL, MIN, MAX),
            min_v_par,
            min_v_seq;
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  min_v_par = getColumnMin_par(matr, ROW, COL);
  if (rank == 0) {
    min_v_seq = getColumnMin_seq(matr, ROW, COL);
    ASSERT_EQ(min_v_par, min_v_seq);
  }
}

TEST(Min_Values_Columns_Matrix, Test_min_positive_single_number_matrix) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> distrib(1, 100);
  const int ROW = 2,
            COL = 3,
            NUM = distrib(gen);
  std::vector<int> matr = { NUM, NUM, NUM, NUM, NUM, NUM },
                   min_v_par,
                   min_v_seq;
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  min_v_par = getColumnMin_par(matr, ROW, COL);
  if (rank == 0) {
    min_v_seq = getColumnMin_seq(matr, ROW, COL);
    ASSERT_EQ(min_v_par, min_v_seq);
  }
}

TEST(Min_Values_Columns_Matrix, Test_zero_only_matrix) {
  const int ROW = 2,
            COL = 3,
            NUM = 0;
  std::vector<int> matr = { NUM, NUM, NUM, NUM, NUM, NUM },
                   min_v_par,
                   min_v_seq;
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  min_v_par = getColumnMin_par(matr, ROW, COL);
  if (rank == 0) {
    min_v_seq = getColumnMin_seq(matr, ROW, COL);
    ASSERT_EQ(min_v_par, min_v_seq);
  }
}

TEST(Min_Values_Columns_Matrix, Test_min_negative_single_number_matrix) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> distrib(-100, -1);
  const int ROW = 2,
            COL = 3,
            NUM = distrib(gen);
  std::vector<int> matr = { NUM, NUM, NUM, NUM, NUM, NUM },
                   min_v_par,
                   min_v_seq;
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  min_v_par = getColumnMin_par(matr, ROW, COL);
  if (rank == 0) {
    min_v_seq = getColumnMin_seq(matr, ROW, COL);
    ASSERT_EQ(min_v_par, min_v_seq);
  }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
