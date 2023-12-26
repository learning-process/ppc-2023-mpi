//  Copyright 2023 Ryabkov Vladislav

#include <gtest/gtest.h>
#include "./strip_vertical_scheme.h"

TEST(Ribbon_Vertical_MPI, A_5x5_B_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_res = nullptr,
      *sequential_res = nullptr;
  int A_rows = 5, A_columns = 5, B_rows = 5;
  if (rank == 0) {
    A = get_random_matrix(A_rows, A_columns);
    parallel_res = init_empty_matrix(A_rows);
    sequential_res = init_empty_matrix(A_rows);
    B = get_random_matrix(B_rows, 1);
  } else {
    B = new int[B_rows];
  }
  MPI_Bcast(B, B_rows, MPI_INT, 0, MPI_COMM_WORLD);
  parallel_matrix_multiplication(A, A_rows, A_columns, B, B_rows, parallel_res);
  if (rank == 0) {
    sequential_matrix_multiplication(A, A_rows, A_columns, B, B_rows,
                                     sequential_res);
    bool flag = true;
    for (int i = 0; i < A_rows; i++) {
      if (parallel_res[i] != sequential_res[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_10x5_B_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_res = nullptr,
      *sequential_res = nullptr;
  int A_rows = 10, A_columns = 5, B_rows = 5;
  if (rank == 0) {
    A = get_random_matrix(A_rows, A_columns);
    parallel_res = init_empty_matrix(A_rows);
    sequential_res = init_empty_matrix(A_rows);
    B = get_random_matrix(B_rows, 1);
  } else {
    B = new int[B_rows];
  }
  MPI_Bcast(B, B_rows, MPI_INT, 0, MPI_COMM_WORLD);
  parallel_matrix_multiplication(A, A_rows, A_columns, B, B_rows, parallel_res);
  if (rank == 0) {
    sequential_matrix_multiplication(A, A_rows, A_columns, B, B_rows,
                                     sequential_res);
    bool flag = true;
    for (int i = 0; i < A_rows; i++) {
      if (parallel_res[i] != sequential_res[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_15x20_B_20) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_res = nullptr,
      *sequential_res = nullptr;
  int A_rows = 15, A_columns = 20, B_rows = 20;
  if (rank == 0) {
    A = get_random_matrix(A_rows, A_columns);
    parallel_res = init_empty_matrix(A_rows);
    sequential_res = init_empty_matrix(A_rows);
    B = get_random_matrix(B_rows, 1);
  } else {
    B = new int[B_rows];
  }
  MPI_Bcast(B, B_rows, MPI_INT, 0, MPI_COMM_WORLD);
  parallel_matrix_multiplication(A, A_rows, A_columns, B, B_rows, parallel_res);
  if (rank == 0) {
    sequential_matrix_multiplication(A, A_rows, A_columns, B, B_rows,
                                     sequential_res);
    bool flag = true;
    for (int i = 0; i < A_rows; i++) {
      if (parallel_res[i] != sequential_res[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_20x40_B_40) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_res = nullptr,
      *sequential_res = nullptr;
  int A_rows = 20, A_columns = 40, B_rows = 40;
  if (rank == 0) {
    A = get_random_matrix(A_rows, A_columns);
    parallel_res = init_empty_matrix(A_rows);
    sequential_res = init_empty_matrix(A_rows);
    B = get_random_matrix(B_rows, 1);
  } else {
    B = new int[B_rows];
  }
  MPI_Bcast(B, B_rows, MPI_INT, 0, MPI_COMM_WORLD);
  parallel_matrix_multiplication(A, A_rows, A_columns, B, B_rows, parallel_res);
  if (rank == 0) {
    sequential_matrix_multiplication(A, A_rows, A_columns, B, B_rows,
                                     sequential_res);
    bool flag = true;
    for (int i = 0; i < A_rows; i++) {
      if (parallel_res[i] != sequential_res[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_30x60_B_60) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_res = nullptr,
      *sequential_res = nullptr;
  int A_rows = 30, A_columns = 60, B_rows = 60;
  if (rank == 0) {
    A = get_random_matrix(A_rows, A_columns);
    parallel_res = init_empty_matrix(A_rows);
    sequential_res = init_empty_matrix(A_rows);
    B = get_random_matrix(B_rows, 1);
  } else {
    B = new int[B_rows];
  }
  MPI_Bcast(B, B_rows, MPI_INT, 0, MPI_COMM_WORLD);
  parallel_matrix_multiplication(A, A_rows, A_columns, B, B_rows, parallel_res);
  if (rank == 0) {
    sequential_matrix_multiplication(A, A_rows, A_columns, B, B_rows,
                                     sequential_res);
    bool flag = true;
    for (int i = 0; i < A_rows; i++) {
      if (parallel_res[i] != sequential_res[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_1x400_B_400) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_res = nullptr,
      *sequential_res = nullptr;
  int A_rows = 1, A_columns = 400, B_rows = 400;
  if (rank == 0) {
    A = get_random_matrix(A_rows, A_columns);
    parallel_res = init_empty_matrix(A_rows);
    sequential_res = init_empty_matrix(A_rows);
    B = get_random_matrix(B_rows, 1);
  } else {
    B = new int[B_rows];
  }
  MPI_Bcast(B, B_rows, MPI_INT, 0, MPI_COMM_WORLD);
  parallel_matrix_multiplication(A, A_rows, A_columns, B, B_rows, parallel_res);
  if (rank == 0) {
    sequential_matrix_multiplication(A, A_rows, A_columns, B, B_rows,
                                     sequential_res);
    bool flag = true;
    for (int i = 0; i < A_rows; i++) {
      if (parallel_res[i] != sequential_res[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
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
    int exec = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec;
}
