// Copyright 2023 Borovkov Sergey
#include <gtest/gtest.h>
#include <mpi.h>
#include <cmath>
#include <iostream>
#include "./alg.h"



void test(double (*f)(double), double a, double b, int n) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double parrResult = integrateParallel(f, a, b, n);

    if (rank == 0) {
        // std::cout << "\nparrResult = " << parrResult << '\n';
        double seqResult = integrateSequential(f, a, b, n);
        // std::cout << "\nseqResult =  " << seqResult << '\n';
        EXPECT_NEAR(parrResult, seqResult, 0.00001);
    }
}

TEST(RectIntegration, RectIntegration_polynom) {
    auto f = [](double x) { return pow(x, 3) + pow(x, 2) + x; };
    double a = 1, b = 2;
    int n = 500;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_exp) {
    auto f = [](double x) { return exp(x); };
    double a = 0, b = 10;
    int n = 450;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_sin) {
    auto f = [](double x) { return sin(x); };
    double a = -10, b = 10;
    int n = 10;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_cos) {
    auto f = [](double x) { return cos(x); };
    double a = 10, b = 20;
    int n = 100;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_complex) {
    auto f = [](double x) { return x * sin(2 * x); };
    double a = -3, b = 100;
    int n = 10000;

    test(f, a, b, n);
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
