#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "most_different.hpp"

TEST(MPI_TESTS, Test_small) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{1, 2, 3, 4, 5, 30, -25, 10, 25, -1};
  int resultParallel = parallelFindMostDifferent(arr);

  if (rank == 0) {
    int resultSequential = sequentialFindMostDifferent(arr);
    ASSERT_EQ(resultParallel, resultSequential);
  }
}

TEST(MPI_TESTS, Test_all_zero) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int resultParallel = parallelFindMostDifferent(arr);

  if (rank == 0) {
    int resultSequential = sequentialFindMostDifferent(arr);
    ASSERT_EQ(resultParallel, resultSequential);
  }
}

TEST(MPI_TESTS, Test_less_two) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{0};
  int resultParallel = parallelFindMostDifferent(arr);

  if (rank == 0) {
    int resultSequential = sequentialFindMostDifferent(arr);
    ASSERT_EQ(resultParallel, resultSequential);
  }
}

TEST(MPI_TESTS, Test_end) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{1, 2, 3, 4, 5, 10, 25, -1, 30, -25};
  int resultParallel = parallelFindMostDifferent(arr);

  if (rank == 0) {
    int resultSequential = sequentialFindMostDifferent(arr);
    ASSERT_EQ(resultParallel, resultSequential);
  }
}

TEST(MPI_TESTS, Test_random) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t sizeArr = 1'000'000;
  const int lowValue = -100'000;
  const int upValue = 100'000;

  std::vector<int> arr(createRandomArray(sizeArr, lowValue, upValue));
  int resultParallel = parallelFindMostDifferent(arr);

  if (rank == 0) {
    int resultSequential = sequentialFindMostDifferent(arr);
    ASSERT_EQ(resultParallel, resultSequential);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  return RUN_ALL_TESTS();
}