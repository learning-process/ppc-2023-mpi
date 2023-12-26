// Copyright 2023 Mamaeva Olga
#include <gtest/gtest.h>
#include <mpi.h>
#include "./ribbon_vertical.h"

TEST(Ribbon_Vertical_MPI, A_6x5_B_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_C = nullptr,
      *sequential_C = nullptr;
  int ARows = 6, ACols = 5, BRows = 5;
  if (rank == 0) {
    A = getRandomMatrix(ARows, ACols);
    parallel_C = initEmptyMatrix(ARows);
    sequential_C = initEmptyMatrix(ARows);
    B = getRandomMatrix(BRows, 1);
  } else {
    B = new int[BRows];
  }
  MPI_Bcast(B, BRows, MPI_INT, 0, MPI_COMM_WORLD);
  ParallelMatrixMultiplication(A, ARows, ACols, B, BRows, parallel_C);
  if (rank == 0) {
    SequentialMatrixMultiplication(A, ARows, ACols, B, BRows, sequential_C);
    bool flag = true;

    for (int i = 0; i < ARows; i++) {
      if (parallel_C[i] != sequential_C[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_10x4_B_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_C = nullptr,
      *sequential_C = nullptr;
  int ARows = 10, ACols = 4, BRows = 4;
  if (rank == 0) {
    A = getRandomMatrix(ARows, ACols);
    parallel_C = initEmptyMatrix(ARows);
    sequential_C = initEmptyMatrix(ARows);
    B = getRandomMatrix(BRows, 1);
  } else {
    B = new int[BRows];
  }
  MPI_Bcast(B, BRows, MPI_INT, 0, MPI_COMM_WORLD);
  ParallelMatrixMultiplication(A, ARows, ACols, B, BRows, parallel_C);
  if (rank == 0) {
    SequentialMatrixMultiplication(A, ARows, ACols, B, BRows, sequential_C);
    bool flag = true;

    for (int i = 0; i < ARows; i++) {
      if (parallel_C[i] != sequential_C[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_12x24_B_24) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_C = nullptr,
      *sequential_C = nullptr;
  int ARows = 12, ACols = 24, BRows = 24;
  if (rank == 0) {
    A = getRandomMatrix(ARows, ACols);
    parallel_C = initEmptyMatrix(ARows);
    sequential_C = initEmptyMatrix(ARows);
    B = getRandomMatrix(BRows, 1);
  } else {
    B = new int[BRows];
  }
  MPI_Bcast(B, BRows, MPI_INT, 0, MPI_COMM_WORLD);
  ParallelMatrixMultiplication(A, ARows, ACols, B, BRows, parallel_C);
  if (rank == 0) {
    SequentialMatrixMultiplication(A, ARows, ACols, B, BRows, sequential_C);
    bool flag = true;

    for (int i = 0; i < ARows; i++) {
      if (parallel_C[i] != sequential_C[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_21x43_B_43) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_C = nullptr,
      *sequential_C = nullptr;
  int ARows = 21, ACols = 43, BRows = 43;
  if (rank == 0) {
    A = getRandomMatrix(ARows, ACols);
    parallel_C = initEmptyMatrix(ARows);
    sequential_C = initEmptyMatrix(ARows);
    B = getRandomMatrix(BRows, 1);
  } else {
    B = new int[BRows];
  }
  MPI_Bcast(B, BRows, MPI_INT, 0, MPI_COMM_WORLD);
  ParallelMatrixMultiplication(A, ARows, ACols, B, BRows, parallel_C);
  if (rank == 0) {
    SequentialMatrixMultiplication(A, ARows, ACols, B, BRows, sequential_C);
    bool flag = true;

    for (int i = 0; i < ARows; i++) {
      if (parallel_C[i] != sequential_C[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

TEST(Ribbon_Vertical_MPI, A_1x500_B_500) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *A = nullptr, *B = nullptr, *parallel_C = nullptr,
      *sequential_C = nullptr;
  int ARows = 1, ACols = 500, BRows = 500;
  if (rank == 0) {
    A = getRandomMatrix(ARows, ACols);
    parallel_C = initEmptyMatrix(ARows);
    sequential_C = initEmptyMatrix(ARows);
    B = getRandomMatrix(BRows, 1);
    // printf("Matrix B\n");
    // for (int i = 0; i < ACols; i++) {
    //  printf("%d ", B[i]);
    //  printf("\n");
    //}
    // fflush(stdout);
  } else {
    B = new int[BRows];
  }
  MPI_Bcast(B, BRows, MPI_INT, 0, MPI_COMM_WORLD);
  double start = MPI_Wtime();
  ParallelMatrixMultiplication(A, ARows, ACols, B, BRows, parallel_C);
  double end = MPI_Wtime();
  double ptime = end - start;
  if (rank == 0) {
    // printf("Matrix A\n");
    // for (int i = 0; i < ARows; i++) {
    //  for (int j = 0; j < ACols; j++) {
    //    printf("%d ", A[j + i * ACols]);
    //  }
    //  printf("\n");
    //}
    // fflush(stdout);
    start = MPI_Wtime();
    SequentialMatrixMultiplication(A, ARows, ACols, B, BRows, sequential_C);
    end = MPI_Wtime();
    double stime = end - start;
    // printf("Matrix sequential_C\n");
    // for (int i = 0; i < ARows; i++) {
    //    printf("%d ", sequential_C[i]);
    //}
    //  printf("\n");
    // fflush(stdout);

    //      printf("Matrix parallel_C\n");
    // for (int i = 0; i < ARows; i++) {
    //  printf("%d ", parallel_C[i]);
    //}
    // printf("\n");
    // fflush(stdout);
    std::cout << "The sequential process took " << stime << std::fixed
              << " seconds to run." << std::endl;
    std::cout << "The parallel process took " << ptime << std::fixed
              << " seconds to run." << std::endl;
    std::cout << "The speedup is " << stime / ptime << " times." << std::endl;
    bool flag = true;

    for (int i = 0; i < ARows; i++) {
      if (parallel_C[i] != sequential_C[i]) {
        flag = false;
      }
    }
    ASSERT_EQ(flag, true);
  }
}

int main(int argc, char **argv) {
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
