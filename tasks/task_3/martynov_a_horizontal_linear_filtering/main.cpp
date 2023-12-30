// Copyright 2023 Martynov Aleksandr
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "../tasks/task_3/martynov_a_horizontal_linear_filtering/horizontal_linear_filtering.h"

TEST(Horizontal_filltartion, try_3_rows_3_cols) {
  int myrank, rows = 3, cols = 3;
  std::vector<unsigned char> image = GetImage(rows, cols),
    result_parallel, result_main;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  result_parallel = toFiltA(image, rows, cols);
  if (myrank == 0) {
    result_main = toFiltB(image, rows, cols);
    ASSERT_EQ(result_parallel, result_main);
  }
}

TEST(Horizontal_filltartion, try_6_rows_3_cols) {
  int myrank, rows = 6, cols = 3;
  std::vector<unsigned char> image = GetImage(rows, cols),
    result_parallel, result_main;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  result_parallel = toFiltA(image, rows, cols);

  if (myrank == 0) {
    result_main = toFiltB(image, rows, cols);
    ASSERT_EQ(result_parallel, result_main);
  }
}

TEST(Horizontal_filltartion, try_5_rows_7_cols) {
  int myrank, rows = 5, cols = 7;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  std::vector<unsigned char> image = GetImage(rows, cols),
    result_parallel, result_main;

  result_parallel = toFiltA(image, rows, cols);

  if (myrank == 0) {
    result_main = toFiltB(image, rows, cols);
    ASSERT_EQ(result_parallel, result_main);
  }
}

TEST(Horizontal_filltartion, try_6_rows_1_cols) {
  int myrank, rows = 6, cols = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  std::vector<unsigned char> image = GetImage(rows, cols),
    result_parallel, result_main;

  result_parallel = toFiltA(image, rows, cols);

  if (myrank == 0) {
    result_main = toFiltB(image, rows, cols);
    ASSERT_EQ(result_parallel, result_main);
  }
}

TEST(Horizontal_filltartion, try_1_rows_6_cols) {
  int myrank, rows = 1, cols = 6;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  std::vector<unsigned char> image = GetImage(rows, cols),
    result_parallel, result_main;
  result_parallel = toFiltA(image, rows, cols);

  if (myrank == 0) {
    result_main = toFiltB(image, rows, cols);
    ASSERT_EQ(result_parallel, result_main);
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
