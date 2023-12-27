// Copyright 2023 Mironov Ilya
#include <gtest/gtest.h>
#include <mpi.h>

#include "./trapezoid_method.h"

#define EPS 0.03

TEST(TRAPEZOIDAL_RULE, Plus) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0, ya = 6.0, xb = 3.0, yb = 9.0;
    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, plus);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, plus);
        ASSERT_NEAR(parallelResult, sequentialResult, EPS);
    }
}

TEST(TRAPEZOIDAL_RULE, Minus) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0, ya = 6.0, xb = 3.0, yb = 9.0;
    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, minus);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, minus);
        ASSERT_NEAR(parallelResult, sequentialResult, EPS);
    }
}

TEST(TRAPEZOIDAL_RULE, Multiple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0, ya = 6.0, xb = 3.0, yb = 9.0;
    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, multiple);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, multiple);
        ASSERT_NEAR(parallelResult, sequentialResult, EPS);
    }
}

TEST(TRAPEZOIDAL_RULE, Division) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0, ya = 6.0, xb = 3.0, yb = 9.0;
    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, division);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, division);
        ASSERT_NEAR(parallelResult, sequentialResult, EPS);
    }
}

TEST(TRAPEZOIDAL_RULE, QuadSum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0, ya = 6.0, xb = 3.0, yb = 9.0;
    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, quadSum);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, quadSum);
        ASSERT_NEAR(parallelResult, sequentialResult, EPS);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    int rank = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
