// Copyright 2023 Korablev Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./matrix_mult.h"

TEST(matrix_test, operator_plus_test) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        const int n = 5;
        matrix<double> a(n, n), b(n, n);
        a.fillMatrix(generateRisingValue, 0, 0, 0);
        b.fillMatrix(generateFallingValue, 0, 0, 0);
        matrix<double> c = a + b;
        bool ok = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (c[i][j] != 0)
                    ok = false;
        ASSERT_TRUE(ok);
    }
}

TEST(cannons_mult_test, mult_test_1) {
    const int n = 10;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        matrix<double> c = a * b;
        ASSERT_TRUE(comp(ans, c));
    }
}

TEST(cannons_mult_test, mult_test_2) {
    const int n = 4;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        matrix<double> c = a * b;
        ASSERT_TRUE(comp(ans, c));
    }
}

TEST(cannons_mult_test, mult_test_3) {
    const int n = 40;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        matrix<double> c = a * b;
        ASSERT_TRUE(comp(ans, c));
    }
}

TEST(cannons_mult_test, mult_test_4) {
    const int n = 20;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        matrix<double> c = a * b;
        ASSERT_TRUE(comp(ans, c));
    }
}

int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
