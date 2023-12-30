// Copyright 2023 Kistrimova Ekaterina
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include "task_2/kistrimova_e_reduce/my_reduce.h"

typedef int v_type;
static const MPI_Datatype mpi_v_type = MPI_INT;
static const int v_size_mul = 500;
static const v_type v_el_max = 100;
static const v_type v_el_min = -100;

inline void test_vector_scalar() {
  double res_time[2];
  v_type *a, *b;
  int i, n, proc_num, proc_rank;
  v_type sum, sum_all, sum_real;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  if (proc_rank == 0) {
    n = proc_num * v_size_mul;
    a = new v_type[n];
    b = new v_type[n];
    sum_real = 0;
    for (i = 0; i < n; i++) {
      a[i] = std::rand() % (v_el_max - v_el_min + 1)
        + v_el_min;
      b[i] = std::rand() % (v_el_max - v_el_min + 1)
        + v_el_min;
      sum_real += a[i] * b[i];
    }
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  n = n / proc_num;
  if (proc_rank != 0) {
    a = new v_type[n];
    b = new v_type[n];
  }
  if (proc_rank == 0) {
    MPI_Scatter(a, n, mpi_v_type,
      MPI_IN_PLACE, n, mpi_v_type, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(NULL, n, mpi_v_type,
      a, n, mpi_v_type, 0, MPI_COMM_WORLD);
  }
  if (proc_rank == 0) {
    MPI_Scatter(b, n, mpi_v_type,
      MPI_IN_PLACE, n, mpi_v_type, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(NULL, n, mpi_v_type,
      b, n, mpi_v_type, 0, MPI_COMM_WORLD);
  }

  sum = sum_all = 0;
  for (i = 0; i < n; i++) {
    sum += a[i] * b[i];
  }

  if (proc_rank == 0) {
    res_time[0] = MPI_Wtime();
  }
  MPI_Reduce(&sum, &sum_all, 1, mpi_v_type, MPI_SUM, 0, MPI_COMM_WORLD);
  if (proc_rank == 0) {
    res_time[1] = MPI_Wtime();
    std::cout << "MPI_Reduce time = " << (res_time[1] - res_time[0]) << '\n';
    EXPECT_EQ(sum_real, sum_all);
  }
  // make it different
  sum_all = 0;
  if (proc_rank == 0) {
    res_time[0] = MPI_Wtime();
  }
  EXPECT_EQ(MPI_SUCCESS, my_mpi_reduce(&sum, &sum_all, 1,
    mpi_v_type, MPI_SUM, 0, MPI_COMM_WORLD));
  if (proc_rank == 0) {
    res_time[1] = MPI_Wtime();
    std::cout << "my_mpi_reduce time = " << (res_time[1] - res_time[0]) << '\n';
  }
  if (proc_rank == 0) {
    EXPECT_EQ(sum_real, sum_all);
  }
  delete[] a;
  delete[] b;
}

TEST(Parallel_Operation_Reduce_MPI, test_calc_tree_props) {
  proc_tree_props props;
  int n = 15;
  int* path = new int[n];
  int path_size;

  calculate_tree_props(7, n, &props, path, &path_size);
  EXPECT_EQ(-1, props.parent);
  EXPECT_EQ(3, props.children[0]);
  EXPECT_EQ(11, props.children[1]);
  EXPECT_EQ(1, path_size);
  if (path_size == 1) {
    EXPECT_EQ(7, path[0]);
  }

  calculate_tree_props(9, n, &props, path, &path_size);
  EXPECT_EQ(11, props.parent);
  EXPECT_EQ(8, props.children[0]);
  EXPECT_EQ(10, props.children[1]);
  EXPECT_EQ(3, path_size);
  if (path_size == 3) {
    EXPECT_EQ(7, path[0]);
    EXPECT_EQ(11, path[1]);
    EXPECT_EQ(9, path[2]);
  }

  calculate_tree_props(2, n, &props, path, &path_size);
  EXPECT_EQ(1, props.parent);
  EXPECT_EQ(-1, props.children[0]);
  EXPECT_EQ(-1, props.children[1]);
  EXPECT_EQ(4, path_size);
  if (path_size == 4) {
    EXPECT_EQ(7, path[0]);
    EXPECT_EQ(3, path[1]);
    EXPECT_EQ(1, path[2]);
    EXPECT_EQ(2, path[3]);
  }
  delete[] path;
}

TEST(Parallel_Operation_Reduce_MPI, test1_vector_scalar) {
  test_vector_scalar();
}

TEST(Parallel_Operation_Reduce_MPI, test2_vector_scalar) {
  test_vector_scalar();
}

TEST(Parallel_Operation_Reduce_MPI, test3_vector_scalar) {
  test_vector_scalar();
}

TEST(Parallel_Operation_Reduce_MPI, test4_vector_scalar) {
  test_vector_scalar();
}

TEST(Parallel_Operation_Reduce_MPI, test5_vector_scalar) {
  test_vector_scalar();
}

int main(int argc, char** argv) {
  int world_rank, result;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();
  if (world_rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  } else {
    std::srand(std::time(nullptr));
  }
  result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
