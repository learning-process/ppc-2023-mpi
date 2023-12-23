// Copyright 2023 Kuznetsov Artem
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./contrast_enhancement.h"

TEST(MPI_TESTS, Test_small) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t m = 4;
  const size_t n = 4;
  const uint8_t new_min = 0;
  const uint8_t new_max = 255;

  std::vector<uint8_t> image{10,  50, 100, 150, 200, 16,  0, 54,
                             122, 89, 100, 0,   1,   255, 4, 5};
  std::vector copyImage(image);

  par_increase_contrast(&image, m, n, new_min, new_max, MPI_COMM_WORLD);

  if (rank == 0) {
    uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
    uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

    seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

    ASSERT_EQ(image, copyImage);
  }
}

TEST(MPI_TESTS, Test_all_zero) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t m = 4;
  const size_t n = 4;
  const uint8_t new_min = 0;
  const uint8_t new_max = 255;

  std::vector<uint8_t> image(m * n, 0);
  std::vector copyImage(image);

  par_increase_contrast(&image, m, n, new_min, new_max, MPI_COMM_WORLD);

  if (rank == 0) {
    uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
    uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

    seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

    ASSERT_EQ(image, copyImage);
  }
}

TEST(MPI_TESTS, Test_all_255) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t m = 4;
  const size_t n = 4;
  const uint8_t new_min = 0;
  const uint8_t new_max = 255;

  std::vector<uint8_t> image(m * n, 255);
  std::vector copyImage(image);

  par_increase_contrast(&image, m, n, new_min, new_max, MPI_COMM_WORLD);

  if (rank == 0) {
    uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
    uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

    seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

    ASSERT_EQ(image, copyImage);
  }
}

TEST(MPI_TESTS, Test_empty) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t m = 0;
  const size_t n = 0;
  const uint8_t new_min = 0;
  const uint8_t new_max = 255;

  std::vector<uint8_t> image{};
  std::vector copyImage(image);

  par_increase_contrast(&image, m, n, new_min, new_max, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(image, copyImage);
  }
}

TEST(MPI_TESTS, Test_random) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t m = 1000;
  const size_t n = 1000;
  const uint8_t new_min = 100;
  const uint8_t new_max = 200;

  std::vector<uint8_t> image = create_random_image(m, n, 0, 255);
  std::vector copyImage(image);

  par_increase_contrast(&image, m, n, new_min, new_max, MPI_COMM_WORLD);

  if (rank == 0) {
    uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
    uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

    seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

    ASSERT_EQ(image, copyImage);
  }
}

int main(int argc, char** argv) {
  int result_code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
