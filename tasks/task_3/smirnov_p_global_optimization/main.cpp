// Copyright 2023 Smirnov Pavel
#include <gtest/gtest.h>
#include "./global_optimization.h"

TEST(Parallel_Operations_MPI, Test_Can_Get_Global_Minimum_Sequentially_F1) {
    double(*func)(double, double) = function_1;
    double xLeftBorder = -10, xRightBorder = 10;
    double yBottomBorder = -10, yTopBorder = 10;
    double r = 2, accuracy = 0.1;
    int maxIterationsCount = 100;

    Point3D trueGlobalMin(0, 0, 0);
    Point3D countedGlobalMin = getGlobalMinimumOnPlane(xLeftBorder,
    xRightBorder,
    yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

    EXPECT_EQ(1, std::abs(countedGlobalMin.x - trueGlobalMin.x) <= accuracy);
    EXPECT_EQ(1, std::abs(countedGlobalMin.y - trueGlobalMin.y) <= accuracy);
}

TEST(Parallel_Operations_MPI, Test_Can_Get_Global_Minimum_Sequentially_F2) {
    double(*func)(double, double) = function_2;
    double xLeftBorder = -10, xRightBorder = 10;
    double yBottomBorder = -10, yTopBorder = 10;
    double r = 2, accuracy = 0.1;
    int maxIterationsCount = 100;

    Point3D trueGlobalMin(5, -10, -13);
    Point3D countedGlobalMin = getGlobalMinimumOnPlane(xLeftBorder,
    xRightBorder,
    yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

    EXPECT_EQ(1, std::abs(countedGlobalMin.x - trueGlobalMin.x) <= accuracy);
    EXPECT_EQ(1, std::abs(countedGlobalMin.y - trueGlobalMin.y) <= accuracy);
}

TEST(Parallel_Operations_MPI, Test_Can_Get_Global_Minimum_Sequentially_F3) {
    double(*func)(double, double) = function_3;
    double xLeftBorder = -10, xRightBorder = 10;
    double yBottomBorder = -10, yTopBorder = 10;
    double r = 2, accuracy = 0.1;
    int maxIterationsCount = 100;

    Point3D trueGlobalMin(5, 3, 0);
    Point3D countedGlobalMin = getGlobalMinimumOnPlane(xLeftBorder,
    xRightBorder,
    yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

    EXPECT_EQ(1, std::abs(countedGlobalMin.x - trueGlobalMin.x) <= accuracy);
    EXPECT_EQ(1, std::abs(countedGlobalMin.y - trueGlobalMin.y) <= accuracy);
}

TEST(Parallel_Operations_MPI, Can_Get_Parallel_Min) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double(*func)(double, double) = function_2;
    double xLeftBorder = -10, xRightBorder = 10;
    double yBottomBorder = -10, yTopBorder = 10;
    double r = 2, accuracy = 0.1;
    int maxIterationsCount = 100;

    Point3D trueGlobalMin(5, -10, -13);
    if (size > 1) {
        Point3D result = getGlobalMinimumOnPlaneParallelly(xLeftBorder,
        xRightBorder,
        yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

        if (rank == 0) {
            EXPECT_EQ(1, std::abs(result.x - trueGlobalMin.x) <= accuracy);
            EXPECT_EQ(1, std::abs(result.y - trueGlobalMin.y) <= accuracy);
        }
    } else {
        Point3D result = getGlobalMinimumOnPlane(xLeftBorder, xRightBorder,
        yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

        if (rank == 0) {
            EXPECT_EQ(1, std::abs(result.x - trueGlobalMin.x) <= accuracy);
            EXPECT_EQ(1, std::abs(result.y - trueGlobalMin.y) <= accuracy);
        }
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