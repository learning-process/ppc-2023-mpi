// Copyright 2023 Derun Andrew
#include <gtest/gtest.h>
#include <mpi.h>

#include <cmath>
#include <vector>

#include "task_3/derun_a_radix_batcher/radix_batcher.h"

TEST(RADIX_BATCHER, TEST_1) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    if (rank == 0) {
      auto vec = getRandomVector(128);
      double* data = vec.data();
      floatRadixSort<double>(&data, 128);
      for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
        EXPECT_TRUE(vec[i] <= vec[i + 1]);
      }
    }
  }
}

TEST(RADIX_BATCHER, TEST_2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    std::vector<double> vec;
    if (rank == 0) {
      vec = getRandomVector(128);
    }
    vec = RadixSortParallel(vec, 128);
    if (rank == 0) {
      for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
        EXPECT_TRUE(vec[i] <= vec[i + 1]);
      }
    }
  }
}

TEST(RADIX_BATCHER, TEST_3) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    std::vector<double> vec;
    if (rank == 0) {
      vec = getRandomVector(pow(2, 8));
    }
    vec = RadixSortParallel(vec, pow(2, 8));
    if (rank == 0) {
      for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
        EXPECT_TRUE(vec[i] <= vec[i + 1]);
      }
    }
  }
}

TEST(RADIX_BATCHER, TEST_4) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    std::vector<double> vec;
    if (rank == 0) {
      vec = getRandomVector(pow(2, 9));
    }
    vec = RadixSortParallel(vec, pow(2, 9));
    if (rank == 0) {
      for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
        EXPECT_TRUE(vec[i] <= vec[i + 1]);
      }
    }
  }
}

TEST(RADIX_BATCHER, TEST_5) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    std::vector<double> vec, vec_s;
    double start, end;
    if (rank == 0) {
      vec = getRandomVector(pow(2, 8));
      vec_s = vec;
    }
    start = MPI_Wtime();
    vec = RadixSortParallel(vec, pow(2, 8));
    end = MPI_Wtime();
    if (rank == 0) {
      double stime = end - start;
      double* data = vec_s.data();
      start = MPI_Wtime();
      floatRadixSort<double>(&data, pow(2, 8));
      end = MPI_Wtime();
      double ptime = end - start;
      for (int i = 0; i < static_cast<int>(vec.size()); i++) {
        EXPECT_DOUBLE_EQ(vec[i], vec_s[i]);
        // std::cout << vec[i] << " " << vec_s[i] << std::endl;
      }
      std::cout << "P time " << ptime << std::endl;
      std::cout << "S time " << stime << std::endl;
      std::cout << "Speedup " << stime / ptime << std::endl;
    }
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
