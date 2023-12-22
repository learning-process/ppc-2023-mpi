// Copyright 2023 kulagin_a
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include "./find_matrix_col_min.h"

typedef int matrix_type;
static const MPI_Datatype mpi_matrix_type = MPI_INT;
static const int matrix_cols_max = 1000;
static const int matrix_rows_max = 1000;
static const matrix_type matrix_element_max = 10000;
static const matrix_type matrix_element_min = -10000;

static inline void test_matrix_col_min() {
  int proc_rank;
  double res_time[2];
  matrix_type* matrix = nullptr;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  int cols, rows;
  if (proc_rank == 0) {
    cols = std::rand() % matrix_cols_max + 1;
    rows = std::rand() % matrix_rows_max + 1;
    matrix = generate_random_matrix(cols, rows, matrix_element_min, matrix_element_max);
  }
  MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  matrix_type* res_parallel;
  if (proc_rank == 0) {
    res_time[0] = MPI_Wtime();
  }
  res_parallel = find_matrix_col_min_parallel(matrix, rows, cols, mpi_matrix_type);
  if (proc_rank == 0) {
    res_time[1] = MPI_Wtime();
    std::cout << "Parallel time = " << (res_time[1] - res_time[0]) << '\n';
    matrix_type* res_real;
    int i;
    res_time[0] = MPI_Wtime();
    res_real = find_matrix_col_min_sequential(matrix, rows, cols);
    res_time[1] = MPI_Wtime();
    std::cout << "Sequential time = " << (res_time[1] - res_time[0]) << '\n';
    for (i = 0; i < cols; i++) {
      EXPECT_EQ(res_real[i], res_parallel[i]);
    }
    delete[] res_real;
  }
  delete[] res_parallel;
  if (matrix != nullptr) {
    delete[] matrix;
  }
}

TEST(Find_Matrix_Col_Min_MPI, test_example) {
  int i, proc_rank;
  matrix_type m[] = { 1, 2, 3, 4,
                      5, 6, 7, 2,
                      3, 4, 1, 6 };
  matrix_type res_real[] = {1, 2, 1, 2};
  matrix_type* res;
  matrix_type* res_parallel;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  res_parallel = find_matrix_col_min_parallel<matrix_type>(m, 3, 4, mpi_matrix_type);
  res = find_matrix_col_min_sequential<matrix_type>(m, 3, 4);
  if (proc_rank == 0) {
    for (i = 0; i < 4; i++) {
      EXPECT_EQ(res_real[i], res[i]);
    }
    EXPECT_EQ(res[0], res_parallel[0]);
    EXPECT_EQ(res[1], res_parallel[1]);
    EXPECT_EQ(res[2], res_parallel[2]);
    EXPECT_EQ(res[3], res_parallel[3]);
  }
  delete[] res;
  delete[] res_parallel;
}

TEST(Find_Matrix_Col_Min_MPI, test1) {
  test_matrix_col_min();
}

TEST(Find_Matrix_Col_Min_MPI, test2) {
  test_matrix_col_min();
}

TEST(Find_Matrix_Col_Min_MPI, test3) {
  test_matrix_col_min();
}

TEST(Find_Matrix_Col_Min_MPI, test4) {
  test_matrix_col_min();
}

TEST(Find_Matrix_Col_Min_MPI, test5) {
  test_matrix_col_min();
}

int main(int argc, char** argv) {
  int world_rank, result;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (world_rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  } else {
    std::srand(std::time(nullptr));
  }
  result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
