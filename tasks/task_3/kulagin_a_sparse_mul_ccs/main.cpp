// Copyright 2023 kulagin_a
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <stdexcept>
#include "./sparse_mul_ccs.h"

static const int rows_min = 10000;
static const int rows_max = 100000;
static const int cols_min = 10000;
static const int cols_max = 100000;
static const int rows_cols_min = 10000;
static const int rows_cols_max = 100000;
static const double value_min = -100.0;
static const double value_max = 100.0;
static const double sparse_density = 0.05;

template<typename T>
static inline void print_arr(const char* name, const T* arr, int size) {
  std::cout << name << " = [";
  if (arr != nullptr && size > 0) {
    int i;
    std::cout << arr[0];
    for (i = 1; i < size; i++) {
      std::cout << ", " << arr[i];
    }
  }
  std::cout << "]\n";
}

static inline void print_props(const sparse_ccs_props& props, const bool& print_arrs = true) {
  std::cout << "rows = " << props.rows << '\n'
            << "cols = " << props.cols << '\n'
            << "vals = " << props.vals << '\n';
  if (print_arrs) {
    print_arr<double>("vals_arr", props.vals_arr, props.vals);
    print_arr<int>("rows_ind_arr", props.rows_ind_arr, props.vals);
    print_arr<int>("cols_count_arr", props.cols_count_arr, props.cols + 1);
  }
}

template<typename T>
static inline void print_matrix(const T* arr, int rows, int cols) {
  int i, j;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      std::cout << arr[j + i * cols] << '\t';
    }
    std::cout << '\n';
  }
}

static inline void test_sparse_mul() {
  int proc_rank;
  double res_time[2];
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  sparse_ccs_props props;
  sparse_ccs m1, m2, m3, m4;
  char matrices_created = 1;
  if (proc_rank == 0) {
    // int m1_rows = rand_range<int>(static_cast<double>(rows_min), static_cast<double>(rows_max));
    // int m2_cols = rand_range<int>(static_cast<double>(cols_min), static_cast<double>(cols_max));
    // int cols_rows = rand_range<int>(static_cast<double>(rows_cols_min), static_cast<double>(rows_cols_max));
    int m1_rows = 100;
    int m2_cols = 100;
    int cols_rows = 100;
    try {
      m1 = rand_sparse(m1_rows, cols_rows, value_min, value_max, sparse_density);
      m2 = rand_sparse(cols_rows, m2_cols, value_min, value_max, sparse_density);
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << '\n';
      matrices_created = 0;
    }
    m1.get_props(&props);
    print_props(props, false);
    m2.get_props(&props);
    print_props(props, false);
  }
  MPI_Bcast(&matrices_created, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
  if (!matrices_created) {
    ADD_FAILURE();
    return;
  }
  if (proc_rank == 0) {
    res_time[0] = MPI_Wtime();
    m3 = m1 * m2;
    res_time[1] = MPI_Wtime();
    m3.get_props(&props);
    print_props(props, false);
    std::cout << "Sequential time = " << (res_time[1] - res_time[0]) << '\n';
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (proc_rank == 0) {
    res_time[0] = MPI_Wtime();
  }
  m4 = parallel_mul(&m1, &m2);
  if (proc_rank == 0) {
    res_time[1] = MPI_Wtime();
  }
  if (proc_rank == 0) {
    m4.get_props(&props);
    print_props(props, false);
    std::cout << "Parallel time = " << (res_time[1] - res_time[0]) << '\n';
    EXPECT_EQ(m3, m4);
  }
}

TEST(Sparse_Mul_MPI, test_props) {
  double matrix[] = { 1, 0, 0, 0,
                       0, 4, 0, 0,
                       2, 0, 6, 0,
                       0, 0, 0, 8,
                       3, 5, 0, 0,
                       0, 0, 7, 0 };

  int i;
  sparse_ccs_props props, props_real;
  sparse_ccs sparse_matrix(matrix, 6, 4), sparse_matrix_real;
  double real_vals_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  int real_rows_ind_arr[] = { 0, 2, 4, 1, 4, 2, 5, 3 };
  int real_cols_count_arr[] = { 0, 3, 5, 7, 8 };
  props_real.rows = 6;
  props_real.cols = 4;
  props_real.vals = 8;
  props_real.vals_arr = real_vals_arr;
  props_real.rows_ind_arr = real_rows_ind_arr;
  props_real.cols_count_arr = real_cols_count_arr;
  EXPECT_EQ(false, sparse_matrix_real == sparse_matrix);
  sparse_matrix_real.set_matrix(&props_real, false);
  EXPECT_EQ(sparse_matrix_real, sparse_matrix);
}

TEST(Sparse_Mul_MPI, test_example) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  double matrix1[] = { 10, 20, 0, 0, 0, 0,
                    0, 30, 0, 40, 0, 0,
                    0, 0, 50, 60, 70, 0,
                    0, 0, 0, 0, 0, 80 };

  double matrix2[] = { 1, 0, 0, 0,
                       0, 4, 0, 0,
                       2, 0, 6, 0,
                       0, 0, 0, 8,
                       3, 5, 0, 0,
                       0, 0, 7, 0 };

  double matrix3[] = { 10, 80, 0, 0,
                       0, 120,  0, 320,
                       310, 350, 300, 480,
                       0, 0, 560, 0 };
  double* matrix_new;
  int i;
  sparse_ccs_props props;
  sparse_ccs sparse_matrix1(matrix1, 4, 6), sparse_matrix2(matrix2, 6, 4);
  sparse_ccs sparse_matrix3(matrix3, 4, 4);
  if (proc_rank == 0) {
    sparse_matrix1.get_props(&props);
    print_props(props);
    sparse_matrix2.get_props(&props);
    print_props(props);
    sparse_matrix3.get_props(&props);
    // print_props(props);
  }
  sparse_matrix1.set_non_zero(1, 3, 5.0);
  matrix_new = sparse_matrix1.construct_matrix();
  EXPECT_EQ(5.0, sparse_matrix1.get(1, 3));
  EXPECT_EQ(5.0, matrix_new[9]);
  delete[] matrix_new;
  sparse_matrix1.set_non_zero(1, 3, 40);
  matrix_new = sparse_matrix1.construct_matrix();
  for (i = 0; i < 24; i++) {
    if (matrix_new[i] != matrix1[i] && proc_rank == 0) {
      std::cout << i << '\n';
      EXPECT_EQ(matrix1[i], matrix_new[i]);
    }
  }
  delete[] matrix_new;
  sparse_ccs sparse_matrix5 = parallel_mul(&sparse_matrix1, &sparse_matrix2);
  if (proc_rank == 0) {
    sparse_ccs sparse_matrix4 = sparse_matrix1 * sparse_matrix2;
    sparse_matrix4.get_props(&props);
    print_props(props);
    sparse_matrix5.get_props(&props);
    print_props(props);
    EXPECT_EQ(sparse_matrix3, sparse_matrix4);
    EXPECT_EQ(sparse_matrix3, sparse_matrix5);
  }
}

TEST(Sparse_Mul_MPI, test_transpose) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  int i, j;
  double matrix1[] = { 10, 20, 0, 0, 0, 0,
                    0, 30, 0, 40, 0, 0,
                    0, 0, 50, 60, 70, 0,
                    0, 0, 0, 0, 0, 80 };

  double matrix2[] = { 10, 0, 0, 0,
                        20, 30, 0, 0,
                        0, 0, 50, 0,
                        0, 40, 60, 0,
                        0, 0, 70, 0,
                        0, 0, 0, 80 };
  sparse_ccs sparse_matrix1(matrix1, 4, 6);
  sparse_ccs_props props;
  sparse_ccs sparse_matrix3 = sparse_matrix1.transpose();
  if (proc_rank == 0) {
    sparse_matrix3.get_props(&props);
    // print_props(props);
  }
  double* matrix3 = sparse_matrix3.construct_matrix();
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 4; j++) {
      int ind = j + i * 4;
      if (matrix2[ind] != matrix3[ind] && proc_rank == 0) {
        std::cout << i << ' ' << j << '\n';
        EXPECT_EQ(matrix2[ind], matrix3[ind]);
      }
    }
  }
  delete[] matrix3;
}

TEST(Sparse_Mul_MPI, test_rand) {
  const double from = 0.0, to = 10.0;
  double tmp = rand_range<double>(from, to);
  EXPECT_EQ(true, tmp >= from);
  EXPECT_EQ(true, tmp <= to);
}

TEST(Sparse_Mul_MPI, test_sparse_rand) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  if (proc_rank == 0) {
    int i;
    const int rows = 10, cols = 10;
    const double from = -10.0, to = 10.0, density = 0.1;
    sparse_ccs rand_m;
    ASSERT_NO_THROW(rand_m = rand_sparse(rows, cols, from, to, density));
    EXPECT_EQ(true, rand_m.is_valid());
    sparse_ccs_props props;
    rand_m.get_props(&props);
    // print_props(props);
    EXPECT_EQ(cols, props.cols);
    EXPECT_EQ(cols, props.rows);
    EXPECT_EQ(true, props.vals > 0);
    EXPECT_EQ(true, props.vals <= (cols * rows) * density);
    for (i = 0; i < props.vals; i++) {
      EXPECT_EQ(false, is_zero(props.vals_arr[i]));
      EXPECT_EQ(true, props.vals_arr[i] >= from);
      EXPECT_EQ(true, props.vals_arr[i] <= to);
      EXPECT_EQ(true, props.rows_ind_arr[i] >= 0);
      EXPECT_EQ(true, props.rows_ind_arr[i] < rows - 1);
    }
    EXPECT_EQ(0, props.cols_count_arr[0]);
    EXPECT_EQ(props.vals, props.cols_count_arr[cols]);
    for (i = 1; i < props.cols + 1; i++) {
      EXPECT_EQ(true, props.cols_count_arr[i] >= 0);
      EXPECT_EQ(true, props.cols_count_arr[i] <= props.vals);
      EXPECT_EQ(true, props.cols_count_arr[i] >= props.cols_count_arr[i-1]);
    }
    double* m = nullptr;
    ASSERT_NO_THROW(m = rand_m.construct_matrix());
    if (m != nullptr) {
      delete[] m;
    }
  }
}


TEST(Sparse_Mul_MPI, test1) {
  test_sparse_mul();
}

TEST(Sparse_Mul_MPI, test2) {
  test_sparse_mul();
}

TEST(Sparse_Mul_MPI, test3) {
  test_sparse_mul();
}

TEST(Sparse_Mul_MPI, test4) {
  test_sparse_mul();
}

TEST(Sparse_Mul_MPI, test5) {
  test_sparse_mul();
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
