// Copyright 2023 Ulyanov Daniil
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./matrix_sum.h"

TEST(matrix_sum, test_1) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> matrix{ 1, 2, 3, 4, 5,
                           6, 7, 8, 9, 10};

  int resPar = parallelSum(matrix);

  if (rank == 0) {
    int resSeq = sequentialSum(matrix);
    ASSERT_EQ(resPar, resSeq);
    ASSERT_EQ(resPar, (1 + 10) * 10 / 2);
  }
}

TEST(matrix_sum, test_2) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> matrix{ };

  int resPar = parallelSum(matrix);

  if (rank == 0) {
    int resSeq = sequentialSum(matrix);
    ASSERT_EQ(resPar, resSeq);
    ASSERT_EQ(resPar, 0);
  }
}

TEST(matrix_sum, test_3) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> matrix(10 * 100);

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 100; j++) {
      matrix[i * 10 + j] = 0 + (std::rand() % (100 - 0));
    }
  }

  int resPar = parallelSum(matrix);

  if (rank == 0) {
    int resSeq = sequentialSum(matrix);
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(matrix_sum, test_4) {
  int rank, num;
  double t1, t2;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> matrix(100 * 100);

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      matrix[i * 100 + j] = i * 100 + j + 1;
    }
  }

  int resPar = parallelSum(matrix);

  if (rank == 0) {
    int resSeq = sequentialSum(matrix);
    ASSERT_EQ(resPar, resSeq);
    ASSERT_EQ(resPar, (1 + 10000) * 10000 / 2);
  }
}

TEST(matrix_sum, test_5) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> vec{432, 134, 423, 123, 567, 896, 457, 586, 213, 534};

  int resPar = parallelSum(vec);

  if (rank == 0) {
    int resSeq = sequentialSum(vec);
    ASSERT_EQ(resPar, resSeq);
  }
}

int main(int argc, char **argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
