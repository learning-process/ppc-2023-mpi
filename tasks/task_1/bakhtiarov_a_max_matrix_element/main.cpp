// Copyright 2023 Bakhtiarov Alexander
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/bakhtiarov_a_max_matrix_element/max_matrix_element.h"
TEST(Parallel_Max_Matrix_Element_MPI, Test_Equal_rows_and_cols) {
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  const int rows = 4;
  const int cols = 4;
  if (world.rank() == 0) {
    global_matrix = generate_random_Matrix(rows, cols);
  }
  int global_max;
  global_max = findMaxOfMatrixParallel(global_matrix, rows, cols);
  if (world.rank() == 0) {
    int local_max = findMaxOfMatrix(global_matrix);
    ASSERT_EQ(local_max, global_max);
  }
}
TEST(Parallel_Max_Matrix_Element_MPI, Test_Different_rows_and_cols) {
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  const int rows = 3;
  const int cols = 7;
  if (world.rank() == 0) {
    global_matrix = generate_random_Matrix(rows, cols);
  }
  int global_max;
  global_max = findMaxOfMatrixParallel(global_matrix, rows, cols);
  if (world.rank() == 0) {
    int local_max = findMaxOfMatrix(global_matrix);
    ASSERT_EQ(local_max, global_max);
  }
}
TEST(Parallel_Max_Matrix_Element_MPI, Test_Zero_rows) {
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  const int rows = 0;
  const int cols = 5;
  if (world.rank() == 0) {
    ASSERT_ANY_THROW(generate_random_Matrix(rows, cols));
  }
}
TEST(Parallel_Max_Matrix_Element_MPI, Test_Zero_cols) {
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  const int rows = 5;
  const int cols = 0;
  if (world.rank() == 0) {
    ASSERT_ANY_THROW(generate_random_Matrix(rows, cols));
  }
}
TEST(Parallel_Max_Matrix_Element_MPI, Test1_negative_in_rows) {
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  const int rows = 3;
  const int cols = 7;
  if (world.rank() == 0) {
    global_matrix = generate_random_Matrix(rows, cols);
    global_matrix[3] = -2;
  }
  int global_max;
  global_max = findMaxOfMatrixParallel(global_matrix, rows, cols);
  if (world.rank() == 0) {
    int local_max = findMaxOfMatrix(global_matrix);
    ASSERT_EQ(local_max, global_max);
  }
}
int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
