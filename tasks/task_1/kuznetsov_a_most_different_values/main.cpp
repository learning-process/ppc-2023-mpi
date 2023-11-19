// Copyright 2023 Kuznetsov Artem
#include <gtest/gtest.h>

#include "./most_different.h"

TEST(MPI_TESTS, Test_small) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{1, 2, 3, 4, 5, 30, -25, 10, 25, -1};
  int result_par = par_find_most_different(arr);

  if (rank == 0) {
    int result_seq = seq_find_most_different(arr);
    ASSERT_EQ(result_par, result_seq);
  }
}

TEST(MPI_TESTS, Test_all_zero) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int result_par = par_find_most_different(arr);

  if (rank == 0) {
    int result_seq = seq_find_most_different(arr);
    ASSERT_EQ(result_par, result_seq);
  }
}

TEST(MPI_TESTS, Test_less_two) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{0};
  int result_par = par_find_most_different(arr);

  if (rank == 0) {
    int result_seq = seq_find_most_different(arr);
    ASSERT_EQ(result_par, result_seq);
  }
}

TEST(MPI_TESTS, Test_end) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> arr{1, 2, 3, 4, 5, 10, 25, -1, 30, -25};
  int result_par = par_find_most_different(arr);

  if (rank == 0) {
    int result_seq = seq_find_most_different(arr);
    ASSERT_EQ(result_par, result_seq);
  }
}

TEST(MPI_TESTS, Test_random) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const size_t sizeArr = 1'000'000;
  const int lowValue = -100'000;
  const int upValue = 100'000;

  std::vector<int> arr(create_random_array(sizeArr, lowValue, upValue));
  int result_par = par_find_most_different(arr);

  if (rank == 0) {
    int result_seq = seq_find_most_different(arr);
    ASSERT_EQ(result_par, result_seq);
  }
}

int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
