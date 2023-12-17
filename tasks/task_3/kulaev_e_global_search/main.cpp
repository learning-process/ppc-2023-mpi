// Copyright 2023 Kulaev Eugene
#include <gtest/gtest.h>
#include <iostream>

#include "./glob_search.h"

double lin(double* x) { return *x; }
double qw(double* x) { return *x * *x; }
double fexp(double* x) { return exp(*x); }
double fcos(double* x) { return cos(*x); }


TEST(Global_Optim_Search, Test1) {
    double a = 0.98;
    double b = 4;
    double eps = 0.0001;

    int part = 10000;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double GlobalOptPar = GetGlobalOptParallel(a, b, lin, part, eps);
    if (ProcRank == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, lin, part, eps);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, eps);
    }
}

TEST(Global_Optim_Search, Test2) {
    double a = 1.5;
    double b = 400;
    double eps = 0.0001;

    int part = 1000;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double GlobalOptPar = GetGlobalOptParallel(a, b, qw, part, eps);
    if (ProcRank == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, qw, part, eps);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, eps);
    }
}

TEST(Global_Optim_Search, Test3) {
    double a = 0.1;
    double b = 6;
    double eps = 0.01;

    int part = 10000;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double GlobalOptPar = GetGlobalOptParallel(a, b, fexp, part, eps);
    if (ProcRank == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, fexp, part, eps);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, eps);
    }
}

TEST(Global_Optim_Search, Test4) {
    double a = 0.1;
    double b = 0.2;
    double eps = 0.0005;

    int part = 10000;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double GlobalOptPar = GetGlobalOptParallel(a, b, fcos, part, eps);
    if (ProcRank == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, fcos, part, eps);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, eps);
    }
}


TEST(Global_Optim_Search, Test5) {
    double a = 1.1;
    double b = 6;
    double eps = 0.01;

    int part = 10000;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double GlobalOptPar = GetGlobalOptParallel(a, b, fexp, part, eps);
    if (ProcRank == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, fexp, part, eps);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, eps);
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
