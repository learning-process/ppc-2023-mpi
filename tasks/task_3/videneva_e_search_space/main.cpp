// Copyright 2023 Videneva Ekaterina

#include <gtest/gtest.h>
#include <iostream>

#include "search_space.h"

double lin(double* x) { return *x; }
double qw(double* x) { return *x * *x; }
double fexp(double* x) { return exp(*x); }
double fcos(double* x) { return cos(*x); }


TEST(Global_Optimization_Search, Test1) {
    double i = 0.98;
    double j = 5;
    double epsilon = 0.0001;

    int part = 10000;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    double GlobalOptimizationPar = GetGlobalOptimizationParallel(i, j, lin, part, epsilon);
    if (Rank == 0) {
        double GlobalOptimizationLin = GetGlobalOptimization(i, j, lin, part, epsilon);
        ASSERT_NEAR(GlobalOptimizationPar, GlobalOptimizationLin, epsilon);
    }
}

TEST(Global_Optimization_Search, Test2) {
    double i = 1.5;
    double j = 400;
    double epsilon = 0.0001;

    int part = 1000;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    double GlobalOptimizationPar = GetGlobalOptimizationParallel(i, j, qw, part, epsilon);
    if (Rank == 0) {
        double GlobalOptimizationLin = GetGlobalOptimization(i, j, qw, part, epsilon);
        ASSERT_NEAR(GlobalOptimizationPar, GlobalOptimizationLin, epsilon);
    }
}

TEST(Global_Optimization_Search, Test3) {
    double i = 0.1;
    double j = 10;
    double epsilon = 0.001;

    int part = 10000;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    double GlobalOptimizationPar = GetGlobalOptimizationParallel(i, j, fexp, part, epsilon);
    if (Rank == 0) {
        double GlobalOptimizationLin = GetGlobalOptimization(i, j, fexp, part, epsilon);
        ASSERT_NEAR(GlobalOptimizationPar, GlobalOptimizationLin, epsilon);
    }
}

TEST(Global_Optimization_Search, Test4) {
    double i = 0.2;
    double j = 0.3;
    double epsilon = 0.0005;

    int part = 10000;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    double GlobalOptimizationPar = GetGlobalOptimizationParallel(i, j, fcos, part, epsilon);
    if (Rank == 0) {
        double GlobalOptimizationLin = GetGlobalOptimization(i, j, fcos, part, epsilon);
        ASSERT_NEAR(GlobalOptimizationPar, GlobalOptimizationLin, epsilon);
    }
}


TEST(Global_Optimization_Search, Test5) {
    double i = 1.5;
    double j = 5;
    double epsilon = 0.01;

    int part = 10000;
    int Rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    double GlobalOptimizationPar = GetGlobalOptimizationParallel(i, j, fexp, part, epsilon);
    if (Rank == 0) {
        double GlobalOptimizationLin = GetGlobalOptimization(i, j, fexp, part, epsilon);
        ASSERT_NEAR(GlobalOptimizationPar, GlobalOptimizationLin, epsilon);
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

  return result_code;
}