// Copyright 2023 Kasimtcev Roman
#include <gtest/gtest.h>

#include "./block_partit.h"


TEST(Parallel_Operations_MPI, Image_1x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int row = 1;
  int col = 8;
  std::vector<double> Image;
  if (rank == 0) {
    Image = randomImage(row, col);
  }
  std::vector<double> res = parllOp(Image, row, col);
  if (rank == 0) {
    std::vector<double> control_res =
        consistentOparation(Image, row, col);
    ASSERT_EQ(control_res, res);
  }
}

TEST(Parallel_Operations_MPI, Image_10x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int row = 1;
  int col = 8;
  std::vector<double> Image;
  if (rank == 0) {
    Image = randomImage(row, col);
  }
  std::vector<double> res = parllOp(Image, row, col);
  if (rank == 0) {
    std::vector<double> control_res =
        consistentOparation(Image, row, col);
    ASSERT_EQ(control_res, res);
  }
}

TEST(Parallel_Operations_MPI, Image_20x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int row = 1;
  int col = 8;
  std::vector<double> Image;
  if (rank == 0) {
    Image = randomImage(row, col);
  }
  std::vector<double> res = parllOp(Image, row, col);
  if (rank == 0) {
    std::vector<double> control_res =
        consistentOparation(Image, row, col);
    ASSERT_EQ(control_res, res);
  }
}

TEST(Parallel_Operations_MPI, Empty_Image) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> Image;
  int row = 1;
  int col = 1;

  ASSERT_ANY_THROW(parllOp(Image, row, col));
}

TEST(Parallel_Operations_MPI, Invalid_Param) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> Image;
  int row = 1;
  int col = 1;
  if (rank == 0) {
    Image = randomImage(row, col);
  }

  ASSERT_ANY_THROW(parllOp(Image, row - 1, col - 1));
}

int main(int argc, char** argv) {
  int code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  code = RUN_ALL_TESTS();
  MPI_Finalize();

  return code;
}
