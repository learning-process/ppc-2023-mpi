// Copyright 2023 Saratova Marina
#include <gtest/gtest.h>
#include "./quick_sort.h"

TEST(MPI_TESTS, Test_Sorting_simple_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 100;
  std::vector<int> vec(size);
  generate_random_values(&vec, 0, 100);

  std::vector<int> first_vector = par_quick_sort(vec);

  if (rank == 0) {
    std::vector<int> second_vector = seq_sorting(vec);
    ASSERT_EQ(true, compare_vectors(first_vector, second_vector));
  }
}

TEST(MPI_TESTS, Test_Sorting_vector_with_negative_numbers) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 100;
  std::vector<int> vec(size);
  generate_random_values(&vec, -100, 100);

  std::vector<int> first_vector = par_quick_sort(vec);

  if (rank == 0) {
    std::vector<int> second_vector = seq_sorting(vec);
    ASSERT_EQ(true, compare_vectors(first_vector, second_vector));
  }
}

TEST(MPI_TESTS, Test_Sorting_big_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 1000;
  std::vector<int> global_vec(size);
  generate_random_values(&global_vec, 0, 100);

  std::vector<int> first_vector = par_quick_sort(global_vec);

  if (rank == 0) {
    std::vector<int> second_vector = seq_sorting(global_vec);
    ASSERT_EQ(true, compare_vectors(first_vector, second_vector));
  }
}

TEST(MPI_TESTS, Test_Sorting_empty_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 0;
  std::vector<int> global_vec(size);
  generate_random_values(&global_vec, 0, 100);

  std::vector<int> first_vector = par_quick_sort(global_vec);

  if (rank == 0) {
    std::vector<int> second_vector = seq_sorting(global_vec);
    ASSERT_EQ(true, compare_vectors(first_vector, second_vector));
  }
}

TEST(MPI_TESTS, Test_Sorting_vector_with_big_interval_of_values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 100;
  std::vector<int> global_vec(size);
  generate_random_values(&global_vec, -10000, 10000);

  std::vector<int> first_vector = par_quick_sort(global_vec);

  if (rank == 0) {
    std::vector<int> second_vector = seq_sorting(global_vec);
    ASSERT_EQ(true, compare_vectors(first_vector, second_vector));
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
