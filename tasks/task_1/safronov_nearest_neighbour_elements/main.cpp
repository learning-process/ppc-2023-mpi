// Copyright 2023 Safronov Mikhail
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <limits>
#include "task_1/safronov_nearest_neighbour_elements/nearest_neighbour_elements.h"


TEST(TESTS, ExampleCase) {
  std::vector<int> vec =
  {4, 9, 13, 45, 1, 7, 3, 6, 1, 22, 90, 4, 17, 8};
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int closest_diff, prev_index, next_index;
  std::tie(closest_diff, prev_index, next_index) =
      findClosestNeighbors(vec, rank);

  if (rank == 0) {
    EXPECT_EQ(closest_diff, 3);
  }
}

TEST(TESTS, EmptyVector) {
  std::vector<int> vec;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int closest_diff, prev_index, next_index;
  std::tie(closest_diff, prev_index, next_index) =
      findClosestNeighbors(vec, rank);

  if (rank == 0) {
    EXPECT_EQ(closest_diff, 0);
  }
}

TEST(TESTS, LargeNumbers) {
  std::vector<int> vec =
  {1000000, 500000, 850000, 900000, 1200000};
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int closest_diff, prev_index, next_index;
  std::tie(closest_diff, prev_index, next_index) =
      findClosestNeighbors(vec, rank);

  if (rank == 0) {
    EXPECT_EQ(closest_diff, 50000);
  }
}

TEST(TESTS, NegativeNumbers) {
  std::vector<int> vec =
  {-5, -10, -3, -8, -12, -15, -6, -2, -9};
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int closest_diff, prev_index, next_index;
  std::tie(closest_diff, prev_index, next_index) =
      findClosestNeighbors(vec, rank);

  if (rank == 0) {
    EXPECT_EQ(closest_diff, 3);
  }
}

TEST(TESTS, EqualElements) {
  std::vector<int> vec = {1, 1, 1, 1, 1};
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int closest_diff, prev_index, next_index;
  std::tie(closest_diff, prev_index, next_index) =
      findClosestNeighbors(vec, rank);

  if (rank == 0) {
    EXPECT_EQ(closest_diff, 0);
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
