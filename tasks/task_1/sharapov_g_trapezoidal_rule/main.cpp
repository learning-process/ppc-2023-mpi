// Copyright 2023 Sharapov Georgiy
#include <mpi.h>
#include <gtest/gtest.h>
#include <cmath>
#include "task_1/sharapov_g_trapezoidal_rule/trapezoidal_rule.h"

TEST(Integral_Trapezoidal_Rule_MPI, Test_Minus_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double err = 0.00001;
    double a = -0.75;
    double b = 0;
    int64_t n = 10000;

    auto function = [](double x) {
        return (3 * x) / sqrt(pow((x + 1), 3));
    };

    double sum = integralTrapezoidalParallel(a, b, n, function);

    if (ProcRank == 0) {
        ASSERT_LT(abs(-3 - sum), err);
    }
}

TEST(Integral_Trapezoidal_Rule_MPI, Test_0) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double err = 0.00001;
    double a = -2.0;
    double b = 2.0;
    int64_t n = 10000;

    auto function = [](double x) {
        return sin(5 * x) / (1 + x * x);
    };

    double sum = integralTrapezoidalParallel(a, b, n, function);

    if (ProcRank == 0) {
        ASSERT_LT(abs(0 - sum), err);
    }
}

TEST(Integral_Trapezoidal_Rule_MPI, Test_PI) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double PI = 3.14159265358979323846;
    double err = 0.00001;
    double a = 0.0;
    double b = 1.0;
    int64_t n = 10000;

    auto function = [](double x) {
        return 4 / (1 + pow(x, 2));
    };

    double sum = integralTrapezoidalParallel(a, b, n, function);

    if (ProcRank == 0) {
        ASSERT_LT(abs(PI - sum), err);
    }
}

TEST(Integral_Trapezoidal_Rule_MPI, Test_PI_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double PI_2 = 1.57079632679489661923;
    double err = 0.00001;
    double a = 0.0;
    double b = PI_2;
    int64_t n = 10000;

    auto function = [](double x) {
        return 2 / (1 + pow(tan(x), sqrt(2)));
    };

    double sum = integralTrapezoidalParallel(a, b, n, function);

    if (ProcRank == 0) {
        ASSERT_LT(abs(PI_2 - sum), err);
    }
}

TEST(Integral_Trapezoidal_Rule_MPI, Test_PI_4) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double PI_4 = 0.78539816339744830962;
    double err = 0.00001;
    double a = 0.0;
    double b = 1.0;
    int64_t n = 10000;

    auto function = [](double x) {
        return std::sqrt(1 - pow(x, 2));
    };

    double sum = integralTrapezoidalParallel(a, b, n, function);

    if (ProcRank == 0) {
        ASSERT_LT(abs(PI_4 - sum), err);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (ProcRank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
