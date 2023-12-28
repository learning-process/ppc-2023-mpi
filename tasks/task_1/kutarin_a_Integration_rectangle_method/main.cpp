// Copyright 2023 Kutarin Aleksandr
#include <gtest/gtest.h>
#include <math.h>
#include "task_1/kutarin_a_Integration_rectangle_method/integrating_rect.h"
#define ESTIMATE 0.01

TEST(Parallel_Integral, sin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double left_border = 0;
    double right_border = 10;
    double result1 = RectangleIntegrate(left_border, right_border,
         0.1, sin_func);
    double result2 = 1.83907;

    if (rank == 0) {
        ASSERT_LT(std::abs(result1-result2), ESTIMATE);
    }
}
TEST(Parallel_Integral, cos) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double left_border = 0;
    double right_border = 10;
    double result1 = RectangleIntegrate(left_border, right_border,
         0.1, cos_func);
    double result2 = -0.544021;

    if (rank == 0) {
        ASSERT_LT(std::abs(result1 - result2), ESTIMATE);
    }
}
TEST(Parallel_Integral, hard_function1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double left_border = 0;
    double right_border = 10;
    double result1 = RectangleIntegrate(left_border, right_border,
         0.1, hardfn_func);
    double result2 = 9.70882;

    if (rank == 0) {
        ASSERT_LT(std::abs(result1 - result2), ESTIMATE);
    }
}
TEST(Parallel_Integral, hard_function2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double left_border = 0;
    double right_border = 10;
    double result1 = RectangleIntegrate(left_border, right_border,
         0.1, hardfn2_func);
    double result2 = 34.21608;

    if (rank == 0) {
        ASSERT_LT(std::abs(result1 - result2), ESTIMATE);
    }
}
TEST(Parallel_Integral, sin_cos) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double left_border = 0;
    double right_border = 10;
    double result1 = RectangleIntegrate(left_border, right_border,
         0.1, sin_cos);
    double result2 = 0.147979;

    if (rank == 0) {
        ASSERT_LT(std::abs(result1 - result2), ESTIMATE);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
