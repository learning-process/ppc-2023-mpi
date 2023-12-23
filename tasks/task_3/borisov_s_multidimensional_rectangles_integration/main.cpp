// Copyright 2023 Borisov Saveliy
#include <gtest/gtest.h>

#include <iostream>

#include "./multidimensional_rectangles_integration.h"

double singleVariableFunction(const std::vector<double>& variables) {
    return exp(variables[0]);
}

double twoVariableFunction(const std::vector<double>& variables) {
    return sin(variables[0]) + cos(variables[1]);
}

double threeVariableFunction(const std::vector<double>& variables) {
    return sin(variables[0]) * cos(variables[1]) + sqrt(variables[2]);
}

TEST(Rectangle_Integrate_MPI_Sequential, Test_Single_Variable) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        double true_value = exp(1)- 1 / exp(1);
        double eps = 1e-3;
        int vars = 1;

        std::vector<int> parts = {1000};
        std::vector<std::pair<double, double>> limits = {{-1.0, 1.0}};

        double numerical_value = integrateSequentially(vars, singleVariableFunction, parts, limits);
        ASSERT_LT(fabs(true_value - numerical_value), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Sequential, Test_Two_Variables) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        double true_value = 4 * sin(1);
        double eps = 1e-1;
        int vars = 2;

        std::vector<int> parts = {100, 100};
        std::vector<std::pair<double, double>> limits = {{-1.0, 1.0}, {-1.0, 1.0}};

        double numerical_value = integrateSequentially(vars, twoVariableFunction, parts, limits);
        ASSERT_LT(fabs(true_value - numerical_value), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Sequential, Test_Three_Variables) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        double true_value = 0.228538683989507;
        double eps = 1e-1;
        int vars = 3;
        std::vector<int> parts = {10, 10, 10};
        std::vector<std::pair<double, double>> limits = {{2.0, 2.5}, {1.0, 2.0}, {0.5, 1.0}};

        double numerical_value = integrateSequentially(vars, threeVariableFunction, parts, limits);
        ASSERT_LT(fabs(true_value - numerical_value), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Parallel, Test_Single_Variable) {
    boost::mpi::communicator world;
    int vars = 1;
    std::vector<int> parts = {1000};
    std::vector<std::pair<double, double>> limits;
    double eps = 1e-6;
    if (world.rank() == 0) {
        limits = {getRandomLimits(-1.0, 1.0)};
    }

    double parallel_result = integrateInParallel(vars, singleVariableFunction, parts, limits);
    if (world.rank() == 0) {
        double sequentially_result = integrateSequentially(vars, singleVariableFunction, parts, limits);
        ASSERT_LT(fabs(parallel_result - sequentially_result), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Parallel, Test_Two_Variables) {
    boost::mpi::communicator world;
    int vars = 2;
    std::vector<int> parts = {100, 100};
    std::vector<std::pair<double, double>> limits;
    double eps = 1e-3;
    if (world.rank() == 0) {
        limits = {getRandomLimits(-5, 5), getRandomLimits(5, 10)};
    }

    double parallel_result = integrateInParallel(vars, twoVariableFunction, parts, limits);
    if (world.rank() == 0) {
        double sequentially_result = integrateSequentially(vars, twoVariableFunction, parts, limits);
        ASSERT_LT(fabs(parallel_result - sequentially_result), eps);
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
