// Copyright 2023 Kanakov Roman
#include <math.h>
#include <gtest/gtest.h>
#include <vector>
#include "./monte_carlo.h"

using std::array;
using std::function;
using std::exp;

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_for_1) {
    auto f = [](const array<double, 1> &x) -> double {
        return 1.0;
    };
    auto region = [](const array<double, 1> &x) -> bool {
        return x[0] >= 0.0 && x[0] <= 0.5;
    };
    array<double[2], 1> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res_parall = multiple_integral_monte_carlo_parallel<1>(
                                f, region, rect, 30000000);
    if (rank == 0) {
        double res = multiple_integral_monte_carlo_sequential<1>(f, region, rect, 30000000);
        ASSERT_NEAR(res_parall, res, 5e-4);
    }
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_for_2) {
    auto f = [](const array<double, 1> &x) -> double {
        return 1.0/(1.0 + x[0]*x[0]);
    };
    auto region = [](const array<double, 1> &x) -> bool {
        return x[0] >= 0.0 && x[0] <= 1.0;
    };
    array<double[2], 1> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res_parall = multiple_integral_monte_carlo_parallel<1>(
                                f, region, rect, 30000000);
    if (rank == 0) {
        double res = multiple_integral_monte_carlo_sequential<1>(f, region, rect, 30000000);
        ASSERT_NEAR(res_parall, res, 4e-4);
    }
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_for_3) {
    auto f = [](const array<double, 2> &x) -> double {
        return 1.0 - x[0]*x[0] - x[1]*x[1];
    };
    auto region = [](const array<double, 2> &x) -> bool {
        return x[0]*x[0] + x[1]*x[1] <= 1.0;
    };
    array<double[2], 2> rect;
    rect[0][0] = -1.0, rect[0][1] = 1.0;
    rect[1][0] = -1.0, rect[1][1] = 1.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res_parall = multiple_integral_monte_carlo_parallel<2>(
                                f, region, rect, 3000000);
    if (rank == 0) {
        double res = multiple_integral_monte_carlo_sequential<2>(f, region, rect, 3000000);
        ASSERT_NEAR(res_parall, res, 4e-3);
    }
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_for_exp) {
    auto f = [](const array<double, 2> &x) -> double {
        return exp(-(x[0]*x[0] + x[1]*x[1]));
    };
    auto region = [](const array<double, 2> &x) -> bool {
        return 0.0 <= x[0] && x[0] <= 1.0 && 0.0 <= x[1] && x[1] <= 1.0;
    };
    array<double[2], 2> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;
    rect[1][0] = 0.0, rect[1][1] = 1.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res_parall = multiple_integral_monte_carlo_parallel<2>(
                                f, region, rect, 30000000);
    if (rank == 0) {
        double res = multiple_integral_monte_carlo_sequential<2>(f, region, rect, 30000000);
        ASSERT_NEAR(res_parall, res, 3e-4);
    }
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_for_5) {
    auto f = [](const array<double, 2> &x) -> double {
        return 1.0 - x[1]*x[1] - x[0]*x[0];
    };
    auto region = [](const array<double, 2> &x) -> bool {
        return x[0]*x[0] + x[1]*x[1] <= 1.0;
    };
    array<double[2], 2> rect;
    rect[0][0] = -1.0, rect[0][1] = 1.0;
    rect[1][0] = -1.0, rect[1][1] = 1.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res_parall = multiple_integral_monte_carlo_parallel<2>(
                                f, region, rect, 3000000);
    if (rank == 0) {
        double res = multiple_integral_monte_carlo_sequential<2>(f, region, rect, 3000000);
        ASSERT_NEAR(res_parall, res, 4e-3);
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
