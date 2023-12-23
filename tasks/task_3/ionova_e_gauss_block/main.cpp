// Copyright 2023 Ionova Ekaterina
#include <gtest/gtest.h>

#include "./gauss_block.h"

TEST(Parallel_Operations_MPI, Image_100x1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rows = 8;
  int cols = 1;
  std::vector<double> Image;
  if (rank == 0) {
    Image = getRandomImage(rows, cols);
  }
  std::vector<double> result = getParallelOperations(Image, rows, cols);
  if (rank == 0) {
    std::vector<double> control_result =
        getSequentialOperations(Image, rows, cols);
    ASSERT_EQ(control_result, result);
  }
}

TEST(Parallel_Operations_MPI, Image_1x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rows = 1;
  int cols = 8;
  std::vector<double> Image;
  if (rank == 0) {
    Image = getRandomImage(rows, cols);
  }
  std::vector<double> result = getParallelOperations(Image, rows, cols);
  if (rank == 0) {
    std::vector<double> control_result =
        getSequentialOperations(Image, rows, cols);
    ASSERT_EQ(control_result, result);
  }
}

TEST(Parallel_Operations_MPI, Image_513x234) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rows = 513;
  int cols = 234;
  std::vector<double> Image;
  if (rank == 0) {
    Image = getRandomImage(rows, cols);
  }
  std::vector<double> result = getParallelOperations(Image, rows, cols);
  if (rank == 0) {
    std::vector<double> control_result =
        getSequentialOperations(Image, rows, cols);
    ASSERT_EQ(control_result, result);
  }
}

TEST(Parallel_Operations_MPI, Image_234x513) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rows = 234;
  int cols = 512;
  std::vector<double> Image;
  if (rank == 0) {
    Image = getRandomImage(rows, cols);
  }
  std::vector<double> result = getParallelOperations(Image, rows, cols);
  if (rank == 0) {
    std::vector<double> control_result =
        getSequentialOperations(Image, rows, cols);
    ASSERT_EQ(control_result, result);
  }
}

TEST(Parallel_Operations_MPI, Image_512x512) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rows = 512;
  int cols = 512;
  std::vector<double> Image;
  if (rank == 0) {
    Image = getRandomImage(rows, cols);
  }
  double parallel_start = MPI_Wtime();
  std::vector<double> result = getParallelOperations(Image, rows, cols);
  if (rank == 0) {
    std::vector<double> control_result =
        getSequentialOperations(Image, rows, cols);
    ASSERT_EQ(control_result, result);
  }
}

TEST(Parallel_Operations_MPI, Empty_Image) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> Image;
  int rows = 1;
  int cols = 1;

  ASSERT_ANY_THROW(getParallelOperations(Image, rows, cols));
}

TEST(Parallel_Operations_MPI, Invalid_Param) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> Image;
  int rows = 1;
  int cols = 1;
  if (rank == 0) {
    Image = getRandomImage(rows, cols);
  }

  ASSERT_ANY_THROW(getParallelOperations(Image, rows - 1, cols - 1));
}

int main(int argc, char** argv) {
  int code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  code = RUN_ALL_TESTS();
  MPI_Finalize();

  return code;
}
