// Copyright 2023 Sobol Lubov

#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./monte_carlo_method_integrate.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

#define DOTS_COUNT 10000
#define ERROR 0.5

double linearFunction(double x) {
    return x;
}

double quadraticFunction(double x) {
    return x * x;
}

TEST(MonteCarloIntegrationMPI, LinearFunctionIntegration) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    const double upperBound = 5.0;
    const double lowerBound = 0.0;

    double integrationResult = calculateMonteCarloIntegral(lowerBound, upperBound, linearFunction, DOTS_COUNT);

    if (currentProcess == 0) {
        ASSERT_NEAR((upperBound - lowerBound) * (upperBound - lowerBound) / 2, integrationResult, ERROR);
    }
}

TEST(MonteCarloIntegrationMPI, LinearFunctionIntegrationWithNegativeLimit) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    const double upperBound = 0.0;
    const double lowerBound = 5.0;

    double integrationResult = calculateMonteCarloIntegral(lowerBound, upperBound, linearFunction, DOTS_COUNT);

    if (currentProcess == 0) {
        ASSERT_NEAR(-(upperBound - lowerBound) * (upperBound - lowerBound) / 2, integrationResult, ERROR);
    }
}

TEST(MonteCarloIntegrationMPI, QuadraticFunctionIntegration) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    const double upperBound = 3.0;
    const double lowerBound = 0.0;

    double integrationResult = calculateMonteCarloIntegral(lowerBound, upperBound, quadraticFunction, DOTS_COUNT);

    if (currentProcess == 0) {
        ASSERT_NEAR((upperBound - lowerBound) * (upperBound - lowerBound) * (upperBound - lowerBound) / 3,
                    integrationResult, ERROR);
    }
}

TEST(MonteCarloIntegrationMPI, TrigonometricFunctionIntegration_1) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    const double pi = 3.141592;
    const double upperBound = pi / 2;
    const double lowerBound = 0.0;

    double integrationResult = calculateMonteCarloIntegral(lowerBound, upperBound, sin, DOTS_COUNT);

    if (currentProcess == 0) {
        ASSERT_NEAR(1, integrationResult, ERROR);
    }
}

TEST(MonteCarloIntegrationMPI, TrigonometricFunctionIntegration_2) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    const double pi = 3.141592;
    const double upperBound = pi;
    const double lowerBound = 0.0;

    double integrationResult = calculateMonteCarloIntegral(lowerBound, upperBound, sin, DOTS_COUNT);

    if (currentProcess == 0) {
        ASSERT_NEAR(2, integrationResult, ERROR);
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
