// Copyright 2023 Musaev Ilgar

#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./method_monte_carlo.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

#define DOTS_COUNT 10000
#define ERROR 0.5

double linear_function(double x) {
    return x;
}

double quadratic_function(double x) {
    return x * x;
}

TEST(Monte_Carlo_Integration_MPI, Linear_function_integration) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double max_lim = 5.0;
    const double min_lim = 0.0;

    double integration_result = monteCarloIntegration(min_lim, max_lim, linear_function, DOTS_COUNT);

    if (rank == 0) {
        ASSERT_NEAR((max_lim - min_lim)*(max_lim - min_lim) / 2, integration_result, ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Linear_function_integration_with_negative_limit) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double max_lim = 0.0;
    const double min_lim = 5.0;

    double integration_result = monteCarloIntegration(min_lim, max_lim, linear_function, DOTS_COUNT);

    if (rank == 0) {
        ASSERT_NEAR(-(max_lim - min_lim)*(max_lim - min_lim) / 2, integration_result, ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Quadratic_function_integration) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double max_lim = 3.0;
    const double min_lim = 0.0;

    double integration_result = monteCarloIntegration(min_lim, max_lim, quadratic_function, DOTS_COUNT);

    if (rank == 0) {
        ASSERT_NEAR((max_lim - min_lim)*(max_lim - min_lim)*(max_lim - min_lim) / 3,
                    integration_result, ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Trigonometric_function_integration) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double pi = 3.141592;
    const double max_lim = pi / 2;
    const double min_lim = 0.0;

    double integration_result = monteCarloIntegration(min_lim, max_lim, sin, DOTS_COUNT);

    if (rank == 0) {
        ASSERT_NEAR(1, integration_result, ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Throws_then_negative_points_count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const double pi = 3.141592;
    const double min_lim = 0.0;
    const double max_lim = pi / 2;

    if (rank == 0) {
        ASSERT_ANY_THROW(monteCarloIntegration(min_lim, max_lim, sin, -DOTS_COUNT));
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
