// Copyright 2023 Alexseev Danila

#include <gtest/gtest.h>
#include "./Monte_Carlo_integral.h"

TEST(MonteCarloIntegral, Pow1Var) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int pointsCount = 1000000;
    std::vector<double> lowerLimits = { 0.0 };
    std::vector<double> upperLimits = { 5.0 };

    double parallelRes = parallelCalculateIntegral(func1VarPow, 1, pointsCount, lowerLimits, upperLimits);

    if (rank == 0) {
        ASSERT_LT(std::abs(parallelRes - 41.66666666666666), 0.5);
    }
}

TEST(MonteCarloIntegral, Cos1Var) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int pointsCount = 1000000;
    std::vector<double> lowerLimits = { 0.0 };
    std::vector<double> upperLimits = { 5.0 };

    double parallelRes = parallelCalculateIntegral(func1VarCos, 1, pointsCount, lowerLimits, upperLimits);

    if (rank == 0) {
        ASSERT_LT(std::abs(parallelRes - (-0.9589242746631386)), 0.05);
    }
}

TEST(MonteCarloIntegral, Exp2Var) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int pointsCount = 1000000;
    std::vector<double> lowerLimits = { 0.0, 0.0 };
    std::vector<double> upperLimits = { 5.0, 5.0 };

    double parallelRes = parallelCalculateIntegral(func2VarExp, 2, pointsCount, lowerLimits, upperLimits);

    if (rank == 0) {
        ASSERT_LT(std::abs(parallelRes - 0.986569), 0.05);
    }
}

TEST(MonteCarloIntegral, Sin2Var) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int pointsCount = 1000000;
    std::vector<double> lowerLimits = { 0.0, 0.0 };
    std::vector<double> upperLimits = { 5.0, 5.0 };

    double parallelRes = parallelCalculateIntegral(func2VarSin, 2, pointsCount, lowerLimits, upperLimits);

    if (rank == 0) {
        ASSERT_LT(std::abs(parallelRes - (-66.082)), 0.5);
    }
}

TEST(MonteCarloIntegral, Sum2Var) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int pointsCount = 1000000;
    std::vector<double> lowerLimits = { 0.0, 0.0 };
    std::vector<double> upperLimits = { 5.0, 5.0 };

    double parallelRes = parallelCalculateIntegral(func2VarSum, 2, pointsCount, lowerLimits, upperLimits);

    if (rank == 0) {
        ASSERT_LT(std::abs(parallelRes), 0.5);
    }
}

int main(int argc, char** argv) {
    int res = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    res = RUN_ALL_TESTS();
    MPI_Finalize();
    return res;
}
