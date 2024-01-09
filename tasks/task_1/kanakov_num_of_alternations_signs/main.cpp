// Copyright 2023 Kanakov Roman
#include <gtest/gtest.h>
#include "./num_of_alternations_signs.h"

TEST(MPI_TESTS, Test_first) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec{111, -1, 111, -1, 0, 1, -1, 0, -1};
  int par_result = par_num_of_alternations_signs(vec);

  if (rank == 0) {
    int seq_result = seq_num_of_alternations_signs(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}

TEST(MPI_TESTS, Test_zeros) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int par_result = par_num_of_alternations_signs(vec);

  if (rank == 0) {
    int seq_result = seq_num_of_alternations_signs(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}

TEST(MPI_TESTS, Test_alternation) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec{1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1,
                       1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
  int par_result = par_num_of_alternations_signs(vec);

  if (rank == 0) {
    int seq_result = seq_num_of_alternations_signs(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}

TEST(MPI_TESTS, Test_random) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int min_val = -1000;
  int max_val = 1000;
  size_t size = 1'000'000;
  std::vector<int> vec(size);

  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<int> dist_gen(min_val, max_val);

  for (auto& val : vec) val = dist_gen(generator);
  int par_result = par_num_of_alternations_signs(vec);

  if (rank == 0) {
    int seq_result = seq_num_of_alternations_signs(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}

TEST(MPI_TESTS, Test_size_zero) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec{};
  int par_result = par_num_of_alternations_signs(vec);

  if (rank == 0) {
    int seq_result = seq_num_of_alternations_signs(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}

int main(int argc, char** argv) {
  int run = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  run = RUN_ALL_TESTS();
  MPI_Finalize();

  return run;
}
