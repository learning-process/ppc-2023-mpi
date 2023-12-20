// Copyright 2023 Veselov Ilya
#include <gtest/gtest.h>
#include <iostream>
#include "./increasing_contrast_img.h"

TEST(Increasing_Contrast_Img, Test_) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t width = 6, height = 6;
  const uint8_t newMin = 0, newMax = 255;
  std::vector<uint8_t> img{23, 56, 89, 123, 45, 67, 200, 34, 78, 90,
                           111, 33, 66, 99, 88, 77, 44, 22, 11, 245,
                           200, 150, 100, 75, 33, 89, 34, 76, 98, 132,
                           210, 177, 199, 240, 138, 72};
  std::vector copy(img);
  parIncContrastImg(&img, width, height, newMin, newMax, MPI_COMM_WORLD);
  if (rank == 0) {
    uint8_t oldMin = *std::min_element(copy.begin(), copy.end());
    uint8_t oldMax = *std::max_element(copy.begin(), copy.end());
    incContrastImg(&copy, oldMin, oldMax, newMin, newMax);
    ASSERT_EQ(img, copy);
  }
}

TEST(Increasing_Contrast_Img, Test_max) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t width = 15, height = 15;
  const uint8_t newMin = 0, newMax = 255;
  std::vector<uint8_t> img(width * height, 255);
  std::vector copy(img);
  parIncContrastImg(&img, width, height, newMin, newMax, MPI_COMM_WORLD);
  if (rank == 0) {
    uint8_t oldMin = *std::min_element(copy.begin(), copy.end());
    uint8_t oldMax = *std::max_element(copy.begin(), copy.end());
    incContrastImg(&copy, oldMin, oldMax, newMin, newMax);
    ASSERT_EQ(img, copy);
  }
}

TEST(Increasing_Contrast_Img, Test_min) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t width = 15, height = 15;
  const uint8_t newMin = 0, newMax = 255;
  std::vector<uint8_t> img(width * height, 0);
  std::vector copy(img);
  parIncContrastImg(&img, width, height, newMin, newMax, MPI_COMM_WORLD);
  if (rank == 0) {
    uint8_t oldMin = *std::min_element(copy.begin(), copy.end());
    uint8_t oldMax = *std::max_element(copy.begin(), copy.end());
    incContrastImg(&copy, oldMin, oldMax, newMin, newMax);
    ASSERT_EQ(img, copy);
  }
}

TEST(Increasing_Contrast_Img, Test_randomImg) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t width = 1500, height = 1500;
  const uint8_t newMin = 127, newMax = 255;
  std::vector<uint8_t> img = generateImg(width, height, 0, 255);
  std::vector copy(img);
  parIncContrastImg(&img, width, height, newMin, newMax, MPI_COMM_WORLD);
  if (rank == 0) {
    uint8_t oldMin = *std::min_element(copy.begin(), copy.end());
    uint8_t oldMax = *std::max_element(copy.begin(), copy.end());
    incContrastImg(&copy, oldMin, oldMax, newMin, newMax);
    ASSERT_EQ(img, copy);
  }
}

TEST(Increasing_Contrast_Img, Test_emptyImg) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t width = 0, height = 0;
  const uint8_t newMin = 0, newMax = 255;
  std::vector<uint8_t> img{};
  std::vector<uint8_t> copy{};
  parIncContrastImg(&img, width, height, newMin, newMax, MPI_COMM_WORLD);
  if (rank == 0) {
    ASSERT_EQ(img, copy);
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int result = 0, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (rank != 0)
    delete listeners.Release(listeners.default_result_printer());
  result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}