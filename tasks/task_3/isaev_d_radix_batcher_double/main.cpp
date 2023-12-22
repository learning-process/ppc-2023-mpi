// Copyright 2023 Isaev Dmitriy
#include <gtest/gtest.h>
#include "./radix_batcher_double.h"

TEST(Parallel_Radix_Batcher, Test1) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int s = 4;
  double check[4] = {2, 3, 0, 5};
  double res[4] = {0, 2, 3, 5};

  double ne[4];
  double *vec1 = new double[4];
  for (int i = 0; i < s; i++) {
    vec1[i] = check[i];
  }

  RadixSortBatcher(&vec1, s);
  for (int i = 0; i < s; i++) {
    ne[i] = vec1[i];
  }

  if (rank == 0) {
    for (int i = 0; i < s; i++) {
      EXPECT_EQ(ne[i], res[i]);
    }
  }
}

TEST(Parallel_Radix_Batcher, Test2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int s = 4;
  double check[4] = {2, -3, 0, 5};
  double res[4] = {-3, 0, 2, 5};

  double ne[4];
  double *vec1 = new double[4];
  for (int i = 0; i < s; i++) {
    vec1[i] = check[i];
  }

  RadixSortBatcher(&vec1, s);
  for (int i = 0; i < s; i++) {
    ne[i] = vec1[i];
  }

  if (rank == 0) {
    for (int i = 0; i < s; i++) {
      EXPECT_EQ(ne[i], res[i]);
    }
  }
}

TEST(Parallel_Radix_Batcher, Test3) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int s = 4;
  double check[4] = {2.1, 3.014, 0.2, 5.7};
  double res[4] = {0.2, 2.1, 3.014, 5.7};

  double ne[4];
  double *vec1 = new double[4];
  for (int i = 0; i < s; i++) {
    vec1[i] = check[i];
  }

  RadixSortBatcher(&vec1, s);
  for (int i = 0; i < s; i++) {
    ne[i] = vec1[i];
  }

  if (rank == 0) {
    for (int i = 0; i < s; i++) {
      EXPECT_EQ(ne[i], res[i]);
    }
  }
}

TEST(Parallel_Radix_Batcher, Test4) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int s = 4;
  double check[4] = {2.1, -3.24, 0.014, 5.6};
  double res[4] = {-3.24, 0.014, 2.1, 5.6};

  double ne[4];
  double *vec1 = new double[4];
  for (int i = 0; i < s; i++) {
    vec1[i] = check[i];
  }

  RadixSortBatcher(&vec1, s);
  for (int i = 0; i < s; i++) {
    ne[i] = vec1[i];
  }

  if (rank == 0) {
    for (int i = 0; i < s; i++) {
      EXPECT_EQ(ne[i], res[i]);
    }
  }
}

TEST(Parallel_Radix_Batcher, Test5) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int s = 3;
  double check[3] = {2.1, -3.24, -5.4};
  double res[3] = {-5.4, -3.24, 2.1};

  double ne[3];
  double *vec1 = new double[3];
  for (int i = 0; i < s; i++) {
    vec1[i] = check[i];
  }

  RadixSortBatcher(&vec1, s);
  for (int i = 0; i < s; i++) {
    ne[i] = vec1[i];
  }

  if (rank == 0) {
    for (int i = 0; i < s; i++) {
      EXPECT_EQ(ne[i], res[i]);
    }
  }
}

int main(int argc, char **argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
