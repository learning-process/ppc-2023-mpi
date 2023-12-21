// Copyright 2023 Shmelev Ivan
#include <gtest/gtest.h>
#include <iostream>
#include "./horizontal_gauss_filter.h"

TEST(MPI_TESTS, Test_Given_Image) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<unsigned char> image = {0, 30, 60, 90, 120, 150, 180, 210, 255};
  const int rows = 3;
  const int cols = 3;
  std::vector<unsigned char> par;
  std::vector<unsigned char> seq;
  par = par_gaussian_filter(image, rows, cols);
  if (rank == 0) {
    seq = seq_gaussian_filter(image, rows, cols);
    ASSERT_EQ(compare_vectors(par, seq), true);
  }
}

TEST(MPI_TESTS, Test_Small_Image) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int rows = 27;
  const int cols = 27;
  std::vector<unsigned char> image = generate_random_image(cols, rows);;
  std::vector<unsigned char> par;
  std::vector<unsigned char> seq;
  par = par_gaussian_filter(image, cols, rows);
  if (rank == 0) {
    seq = seq_gaussian_filter(image, cols, rows);
    ASSERT_EQ(compare_vectors(par, seq), true);
  }
}

TEST(MPI_TESTS, Test_Big_Image) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int rows{ 1980 };
  const int cols{ 720 };
  std::vector<unsigned char> image = generate_random_image(cols, rows);;
  std::vector<unsigned char> par;
  std::vector<unsigned char> seq;
  par = par_gaussian_filter(image, cols, rows);
  if (rank == 0) {
    seq = seq_gaussian_filter(image, cols, rows);
    ASSERT_EQ(compare_vectors(par, seq), true);
  }
}

TEST(MPI_TESTS, Test_Empty_Image) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int rows = 0;
  const int cols = 0;
  std::vector<unsigned char> image = generate_random_image(cols, rows);;
  std::vector<unsigned char> par;
  std::vector<unsigned char> seq;
  par = par_gaussian_filter(image, rows, cols);
  if (rank == 0) {
    seq = seq_gaussian_filter(image, rows, cols);
    ASSERT_EQ(compare_vectors(par, seq), true);
  }
}


TEST(MPI_TESTS, Test_Image_With_One_Column) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int rows = 720;
  const int cols = 1;
  std::vector<unsigned char> image = generate_random_image(cols, rows);;
  std::vector<unsigned char> par;
  std::vector<unsigned char> seq;
  par = par_gaussian_filter(image, rows, cols);
  if (rank == 0) {
    seq = seq_gaussian_filter(image, rows, cols);
    ASSERT_EQ(compare_vectors(par, seq), true);
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
