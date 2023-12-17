// Copyright 2023 Kazantsev Evgeny
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "../tasks/task_2/kazantsev_e_mul_tape_matrix/mul_tape_matrix.h"

TEST(Parallel_tape_matrix_mul, test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 33, m = 33;
  std::vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = GetMatrixRand(n);
    B = GetMatrixRand(n);
  }

  std::vector<int> res = parallelMatrixMul(&A, &B, n, m);

  if (rank == 0) {
    std::vector<int> expected_res = SequentialMulMatrix(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(Parallel_tape_matrix_mul, test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 11, m = 11;
  std::vector<int> A(n * m), B(n * m);
  if (rank == 0) {
    A = GetMatrixRand(n);
    B = GetMatrixRand(n);
  }

  std::vector<int> res = parallelMatrixMul(&A, &B, n, m);

  if (rank == 0) {
    std::vector<int> expected_res = SequentialMulMatrix(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(Parallel_tape_matrix_mul, test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 4, m = 4;
  std::vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = GetMatrixRand(n);
    B = GetMatrixRand(n);
  }

  std::vector<int> res = parallelMatrixMul(&A, &B, n, m);

  if (rank == 0) {
    std::vector<int> expected_res = SequentialMulMatrix(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(Parallel_tape_matrix_mul, test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 5, m = 5;
  std::vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = GetMatrixRand(n);
    B = GetMatrixRand(n);
  }

  std::vector<int> res = parallelMatrixMul(&A, &B, n, m);

  if (rank == 0) {
    std::vector<int> expected_res = SequentialMulMatrix(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(Parallel_tape_matrix_mul, test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 1, m = 1;
  std::vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = GetMatrixRand(n);
    B = GetMatrixRand(n);
  }

  std::vector<int> res = parallelMatrixMul(&A, &B, n, m);

  if (rank == 0) {
    std::vector<int> expected_res = SequentialMulMatrix(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
     int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
