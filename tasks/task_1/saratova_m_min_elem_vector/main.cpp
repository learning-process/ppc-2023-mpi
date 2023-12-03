// Copyright 2023 Saratova Marina
#include <gtest/gtest.h>
#include "./min_elem_vector.h"

TEST(MPI_TESTS, Test_given_vector) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec{1, 2, 3, 4, 5, 30, -25, 10, 25, -2};

  int par_result = par_find_min_elem_vector(vec);

  if (rank == 0) {
    int seq_result = seq_find_min_elem_vector(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}


TEST(MPI_TESTS, Test_random_vector) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = generate_random_vector(100, -1000, 1000);;

  int par_result = par_find_min_elem_vector(vec);

  if (rank == 0) {
    int seq_result = seq_find_min_elem_vector(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}


TEST(MPI_TESTS, Test_big_random_vector) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = generate_random_vector(1000000, -1000, 1000);;

  int par_result = par_find_min_elem_vector(vec);

  if (rank == 0) {
    int seq_result = seq_find_min_elem_vector(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}


TEST(MPI_TESTS, Test_empty_vector) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = generate_random_vector(0, -100, 100);

  int par_result = par_find_min_elem_vector(vec);

  if (rank == 0) {
    int seq_result = seq_find_min_elem_vector(vec);
    ASSERT_EQ(par_result, seq_result);
  }
}

TEST(MPI_TESTS, Test_vector_with_zero_interval) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = generate_random_vector(100, 0, 0);

  int par_result = par_find_min_elem_vector(vec);

  if (rank == 0) {
    int seq_result = seq_find_min_elem_vector(vec);
    ASSERT_EQ(par_result, seq_result);
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
