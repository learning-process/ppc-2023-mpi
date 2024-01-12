// Copyright 2023 Shipitsin Alex
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_vertical.h"

#define EPSILON 0.000001

TEST(Gauss_Vertical, Incorrect_sizes) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 0, cols = 10, vecSize = 10;
    Matrix matrix;
    Matrix vec;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    ASSERT_ANY_THROW(ParallelGauss(matrix, rows, cols, vec, vecSize));
}

TEST(Gauss_Vertical, Correct_Answer_Seq_3x3x3) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    const int rows = 3, cols = 3;
    Matrix mat = {  1, 5, 3,
                    4, 2, -1,
                    -2, 2, 3};
    Matrix vec = {  -3, 2, 0};
    if (procRank == 0) {
        Matrix result = SequentialGauss(mat, rows, cols, vec, rows);
        Matrix exRes = { 7, -8, 10 };
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(result[row], exRes[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, Correct_Answer_Seq_4x4x4) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  const int rows = 4, cols = 4;
  Matrix mat = {1, 2, 3, 4,
                5, 6, 7, 8,
                9, 1, 2, 3,
                4, 5, 1, 2};
  Matrix vec = {3, 7, 5, 1};
  if (procRank == 0) {
    Matrix result = SequentialGauss(mat, rows, cols, vec, rows);
    Matrix exRes = { 0.333333, -0.333333, 0.666667, 0.333333 };
    for (int row = 0; row < rows; row++) {
        ASSERT_NEAR(result[row], exRes[row], EPSILON);
    }
  }
}

TEST(Gauss_Vertical, Correct_Answer_Par_3x3x3) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    const int rows = 3, cols = 3;
    Matrix mat = {  1, 5, 3,
                    4, 2, -1,
                    -2, 2, 3};
    Matrix vec = {  -3, 2, 0};
    Matrix result = ParallelGauss(mat, rows, cols, vec, rows);
    if (procRank == 0) {
      Matrix exRes = SequentialGauss(mat, rows, cols, vec, rows);
      for (int row = 0; row < rows; row++) {
          ASSERT_NEAR(result[row], exRes[row], EPSILON);
      }
  }
}
TEST(Gauss_Vertical, Correct_Answer_Par_4x4x4) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  const int rows = 4, cols = 4;
  Matrix mat = {1, 2, 3, 4,
                5, 6, 7, 8,
                9, 1, 2, 3,
                4, 5, 1, 2};
  Matrix vec = {3, 7, 5, 1};
  Matrix result = ParallelGauss(mat, rows, cols, vec, rows);
  if (procRank == 0) {
    Matrix exRes = SequentialGauss(mat, rows, cols, vec, rows);
    for (int row = 0; row < rows; row++) {
        ASSERT_NEAR(result[row], exRes[row], EPSILON);
    }
  }
}

TEST(Gauss_Vertical, 4x4x4) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 4, cols = 4, vecSize = 4;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, 5x5x5) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 5, cols = 5, vecSize = 5;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, 6x6x6) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 30, cols = 30, vecSize = 30;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, 20x20x20) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 20, cols = 20, vecSize = 20;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}

int main(int argc, char* argv[]) {
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
