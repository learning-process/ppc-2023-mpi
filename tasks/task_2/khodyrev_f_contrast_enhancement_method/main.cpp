// Copyright 2023 Khodyrev Fedor
#include <gtest/gtest.h>
#include <mpi.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "./contrast_enhancement.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(MPI_TESTS, Test_big_random_image_with_even_elements) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 200, cols = 100;

  std::vector<std::vector<int>> image = generate_image(rows, cols);
  std::vector<std::vector<int>> result_par = par_linear_contrast_stretching
    (image, rows, cols);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<std::vector<int>> result_seq = seq_linear_contrast_stretching
      (image, rows, cols);

    EXPECT_TRUE(compareMatrix(result_par, result_seq));
  }
}

TEST(MPI_TESTS, Test_big_random_image_with_odd_elements) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 201, cols = 101;

  std::vector<std::vector<int>> image = generate_image(rows, cols);
  std::vector<std::vector<int>> result_par = par_linear_contrast_stretching
    (image, rows, cols);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<std::vector<int>> result_seq = seq_linear_contrast_stretching
      (image, rows, cols);
    EXPECT_TRUE(compareMatrix(result_par, result_seq));
  }
}

TEST(MPI_TESTS, Test_empty_image) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 0, cols = 0;

  std::vector<std::vector<int>> image = generate_image(rows, cols);
  std::vector<std::vector<int>> result_par = par_linear_contrast_stretching
    (image, rows, cols);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<std::vector<int>> result_seq = seq_linear_contrast_stretching
      (image, rows, cols);
    EXPECT_TRUE(compareMatrix(result_par, result_seq));
  }
}

TEST(MPI_TESTS, Test_given_image) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 10, cols = 20;

    std::vector<std::vector<int>> image = {
        {34, 56, 78, 91, 122, 5, 67, 98, 23, 178, 200, 45,
        76, 99, 203, 255, 13, 53, 87, 231},
        {120, 45, 76, 23, 67, 99, 208, 198, 76, 32, 67, 88,
        11, 93, 127, 255, 31, 77, 100, 54},
        {200, 33, 88, 101, 44, 76, 233, 12, 56, 87, 121, 35,
        68, 98, 88, 250, 87, 177, 200, 45},
        {11, 77, 101, 43, 87, 255, 98, 200, 35, 67, 89, 120,
        33, 78, 99, 210, 97, 123, 212, 176},
        {66, 98, 155, 22, 87, 33, 201, 198, 34, 76, 99, 214,
        65, 77, 211, 122, 78, 44, 99, 88},
        {88, 23, 98, 76, 100, 212, 150, 99, 78, 65, 99, 234,
        87, 101, 31, 199, 23, 100, 154, 45},
        {54, 98, 123, 56, 76, 201, 35, 97, 201, 99, 98, 154,
        32, 77, 109, 198, 54, 98, 176, 21},
        {78, 90, 200, 32, 100, 22, 99, 101, 155, 88, 78, 101,
        200, 77, 97, 187, 22, 45, 99, 255},
        {90, 120, 32, 198, 210, 100, 155, 176, 99, 78, 67, 99,
        201, 45, 98, 166, 34, 56, 99, 121},
        {110, 200, 76, 87, 101, 212, 87, 33, 54, 199, 76, 32,
        77, 98, 155, 45, 99, 78, 33, 66}
    };

  std::vector<std::vector<int>> result_par = par_linear_contrast_stretching
    (image, rows, cols);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<std::vector<int>> result_seq = seq_linear_contrast_stretching
      (image, rows, cols);
    EXPECT_TRUE(compareMatrix(result_par, result_seq));
  }
}

TEST(MPI_TESTS, Test_image_with_one_row) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 1, cols = 300;

  std::vector<std::vector<int>> image = generate_image(rows, cols);
  std::vector<std::vector<int>> result_par = par_linear_contrast_stretching
    (image, rows, cols);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<std::vector<int>> result_seq = seq_linear_contrast_stretching
      (image, rows, cols);
    EXPECT_TRUE(compareMatrix(result_par, result_seq));
  }
}

TEST(MPI_TESTS, Test_image_with_one_column) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 300, cols = 1;

  std::vector<std::vector<int>> image = generate_image(rows, cols);
  std::vector<std::vector<int>> result_par = par_linear_contrast_stretching
    (image, rows, cols);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<std::vector<int>> result_seq = seq_linear_contrast_stretching
      (image, rows, cols);
    EXPECT_TRUE(compareMatrix(result_par, result_seq));
  }
}



int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
