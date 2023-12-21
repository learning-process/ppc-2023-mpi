// Copyright 2023 Konovalov Igor
#include <gtest/gtest.h>
#include "./quick_sort_simple_merge.h"


TEST(quick_sort_simple_merge, small_random_array_test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(nullptr));
  int n = 10;
  int* a = new int[n];
  for (int i = 0; i < n; ++i) a[i] = rand_r();

  quickSortPar(a, n);

  if (rank == 0) {
    EXPECT_TRUE(check(a, n));
  }
  delete[] a;
}

TEST(quick_sort_simple_merge, large_random_array_test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(nullptr));
  int n = 1000;
  int* a = new int[n];
  for (int i = 0; i < n; ++i) a[i] = rand_r();

  quickSortPar(a, n);

  if (rank == 0) {
    EXPECT_TRUE(check(a, n));
  }
  delete[] a;
}

TEST(quick_sort_simple_merge, large_range_array_consistency_test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = 1000;
  int* a = new int[n];
  for (int i = 0; i < n; ++i) a[i] = i;

  quickSortPar(a, n);

  if (rank == 0) {
    for (int i = 0; i < n; ++i) {
      if (a[i] != i) {
        FAIL();
      }
    }
    SUCCEED();
  }
  delete[] a;
}

TEST(quick_sort_simple_merge, very_large_array_test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(nullptr));
  int n = 10000;
  int* a = new int[n];
  for (int i = 0; i < n; ++i) a[i] = rand_r();

  quickSortPar(a, n);

  if (rank == 0) {
    EXPECT_TRUE(check(a, n));
  }
  delete[] a;
}

TEST(quick_sort_simple_merge, very_very_large_array_test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(nullptr));
  int n = 20000;
  int* a = new int[n];
  for (int i = 0; i < n; ++i) a[i] = rand_r();

  quickSortPar(a, n);

  if (rank == 0) {
    EXPECT_TRUE(check(a, n));
  }
  delete[] a;
}

int main(int argc, char** argv) {
  int result = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result = RUN_ALL_TESTS();
  MPI_Finalize();

  return result;
}
