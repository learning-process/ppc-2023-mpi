// Copyright 2023 Savchuk Anton

#include <gtest/gtest.h>
#include <mpi.h>
#include "./quick_sort.h"

TEST(quicksort_simple_merge, test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int si = 10000;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = i;

  parallel_quick_sort(a, si);

  if (rank == 0) {
    bool flag = true;
    for (int i = 0; i < si; ++i) {
      if (a[i] != i) flag = false;
    }
    EXPECT_EQ(flag, true);
  }
  delete[] a;
}

TEST(quicksort_simple_merge, test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 100;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}
TEST(quicksort_simple_merge, test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 1000;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}

TEST(quicksort_simple_merge, test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 1500;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}

TEST(quicksort_simple_merge, test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 3000;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);

    // Get the rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Run the tests only for rank 0
    if (rank == 0) {
        // Run the tests
        int result = RUN_ALL_TESTS();

        // Print the result
        if (result == 0) {
            std::cout << "All tests passed!" << std::endl;
        } else {
            std::cerr << "Some tests failed!" << std::endl;
        }
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
