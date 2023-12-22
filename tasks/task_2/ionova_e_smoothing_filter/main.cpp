// Copyright 2023 Ionova Ekaterina
#include <gtest/gtest.h>

#include "./filter.h"

TEST(TESTS, Test_1) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  int rows = 1;
  int cols = 1;
  int kernel = 3;
  std::vector<uint8_t> img{255};

  auto par_result = parFilter(img, rows, cols, kernel, comm);

  if (rank == 0) {
    auto seq_result = seqFilter(img, rows, cols, kernel);
    ASSERT_EQ(seq_result, par_result);
  }
}

TEST(TESTS, Test_2) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  int rows = 2;
  int cols = 2;
  int kernel = 3;
  std::vector<uint8_t> img{7, 255, 0, 27};

  auto par_result = parFilter(img, rows, cols, kernel, comm);

  if (rank == 0) {
    auto seq_result = seqFilter(img, rows, cols, kernel);
    ASSERT_EQ(seq_result, par_result);
  }
}

TEST(TESTS, Test_4) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  int rows = 4;
  int cols = 4;
  int kernel = 3;
  std::vector<uint8_t> img{150, 30, 100, 30,  20,  7, 0,  0,
                           255, 55, 0,   255, 124, 0, 27, 6};

  auto par_result = parFilter(img, rows, cols, kernel, comm);

  if (rank == 0) {
    auto seq_result = seqFilter(img, rows, cols, kernel);
    ASSERT_EQ(seq_result, par_result);
  }
}

TEST(TESTS, Test_empty) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  int rows = 0;
  int cols = 0;
  int kernel = 3;
  std::vector<uint8_t> img;

  auto par_result = parFilter(img, rows, cols, kernel, comm);

  if (rank == 0) {
    auto seq_result = seqFilter(img, rows, cols, kernel);
    ASSERT_EQ(seq_result, par_result);
  }
}

TEST(TESTS, Test_random) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  int rows = 1000;
  int cols = 1000;
  int kernel = 3;
  std::vector<uint8_t> img(rows * cols);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 255);

  for (auto& pix : img) pix = dist(gen);

  auto par_result = parFilter(img, rows, cols, kernel, comm);

  if (rank == 0) {
    auto seq_result = seqFilter(img, rows, cols, kernel);
    ASSERT_EQ(seq_result, par_result);
  }
}

int main(int argc, char** argv) {
  int result_code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
