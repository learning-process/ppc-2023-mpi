// Copyright 2023 Martynov Aleksandr
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./min_vector_elements.h"

TEST(MPI_TESTS, Test_vector_with_only_positive) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const size_t vec_size = 97;
  int min = 1e+10;
  get_random_vector(vec, vec_size, 0, 100);

  int par_result = get_minimal_elem(vec, vec_size);
  if (rank == 0) {
    for (int q = 0; q < vec_size; q++) {
      if (min > vec[q]) min = vec[q];
    }
    ASSERT_EQ(par_result, min);
  }
}

TEST(MPI_TESTS, Test_vector_with_positive_negative) {
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const size_t vec_size = 97;
  int min = 1e+10;
  get_random_vector(vec, vec_size, -100, 100);

  int par_result = get_minimal_elem(vec, vec_size);
  if (rank == 0) {
    for (int q = 0; q < vec_size; q++) {
      if (min > vec[q]) min = vec[q];
    }
    ASSERT_EQ(par_result, min);
  }
}

TEST(MPI_TESTS, Test_vector_with_only_negative) {
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const size_t vec_size = 97;
  int min = 1e+10;
  get_random_vector(vec, vec_size, -100, 0);

  int par_result = get_minimal_elem(vec, vec_size);
  if (rank == 0) {
    for (int q = 0; q < vec_size; q++) {
      if (min > vec[q]) min = vec[q];
    }
    ASSERT_EQ(par_result, min);
  }
}

TEST(Min_Vector_Elements, Test_several_min) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const size_t vec_size = 10;
  vec = {1, 10, 0, 37, 45, 0, 1, 2, 0, 23};
  int parallel_res = get_minimal_elem(vec, vec_size);
  if (rank == 0) {
    int seq_res = *std::min_element(vec.begin(), vec.end());
    ASSERT_EQ(seq_res, parallel_res);
  }
}

TEST(Min_Vector_Elements, Test_0) {
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t vec_size = 100;
  std::vector<int> vec(vec_size);
  for (int q = 0; q < vec_size; q++)
    vec[q] = 0;
  int parallel_res = get_minimal_elem(vec, vec_size);

  if (rank == 0) {
    int seq_res = *std::min_element(vec.begin(), vec.end());
    ASSERT_EQ(seq_res, parallel_res);
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
  return 0;
}
