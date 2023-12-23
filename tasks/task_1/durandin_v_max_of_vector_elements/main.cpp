// Copyright 2023 Durandin Vladimir
#include <gtest/gtest.h>

#include "task_1/durandin_v_max_of_vector_elements/vector_max_element.h"

TEST(MPI_TESTS, empty_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int64_t> vector;

  const int64_t par_max = par_get_max_element_of_vector(vector);

  if (rank == 0) {
    int64_t serial_max = sub_get_max_element_of_vector(vector);
    ASSERT_EQ(serial_max, par_max);
  }
}

TEST(MPI_TESTS, vector_of_zeros) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int64_t> vector{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const int64_t par_max = par_get_max_element_of_vector(vector);

  if (rank == 0) {
    int64_t serial_max = sub_get_max_element_of_vector(vector);
    ASSERT_EQ(serial_max, par_max);
  }
}

TEST(MPI_TESTS, vector_with_a_tail) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int64_t> vector(
      create_random_vector(10'007ll, -100ll,
                           100ll));  // 10'007 is a prime number
  const int64_t par_max = par_get_max_element_of_vector(vector);

  if (rank == 0) {
    int64_t serial_max = sub_get_max_element_of_vector(vector);
    ASSERT_EQ(serial_max, par_max);
  }
}

TEST(MPI_TESTS, vector_with_even_size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int64_t> vector(create_random_vector(100'000ll, -100ll, 100ll));
  const int64_t par_max = par_get_max_element_of_vector(vector);

  if (rank == 0) {
    int64_t serial_max = sub_get_max_element_of_vector(vector);
    ASSERT_EQ(serial_max, par_max);
  }
}

TEST(MPI_TESTS, vector_with_negative_values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int64_t> vector(create_random_vector(100'000ll, -10'000ll, -1ll));
  const int64_t par_max = par_get_max_element_of_vector(vector);

  if (rank == 0) {
    int64_t serial_max = sub_get_max_element_of_vector(vector);
    ASSERT_EQ(serial_max, par_max);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  int exec_code = RUN_ALL_TESTS();
  MPI_Finalize();
  return exec_code;
}
