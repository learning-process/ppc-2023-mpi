// Copyright 2023 Kachalov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_jordan.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(GaussJordanTests, TestSequentialSolving) {
    std::vector<std::vector<double>> A = {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}};
    std::vector<double> b = {8, -11, -3};
    
    if (world.rank() == 0) {
        std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
        ASSERT_EQ(sequentialSolution.size(), b.size());
    }
}

TEST(GaussJordanTests, TestParallelSolving) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<std::vector<double>> A = {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}};
    std::vector<double> b = {8, -11, -3};

    std::vector<double> parallelSolution = solve_linear_system_parallel(A, b);

    if (world.rank() == 0) {
        std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
        ASSERT_EQ(parallelSolution, sequentialSolution);
    }
}

TEST(GaussJordanTests, TestNoSolution) {
    std::vector<std::vector<double>> A = {{2, -1, 3}, {4, 2, 1}, {-6, 3, -2}};
    std::vector<double> b = {9, 8, 1};

    std::vector<double> parallelSolution = solve_linear_system_parallel(A, b);
    if (world.rank() == 0) {
       std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
       ASSERT_TRUE(sequentialSolution.empty());
       ASSERT_TRUE(parallelSolution.empty());
    }
}

TEST(GaussJordanTests, TestInfiniteSolutions) {
    std::vector<std::vector<double>> A = {{1, 2, 3}, {4, 8, 12}, {7, 14, 21}};
    std::vector<double> b = {6, 18, 33};

    std::vector<double> parallelSolution = solve_linear_system_parallel(A, b);

    if (world.rank() == 0) {
       std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
       ASSERT_EQ(sequentialSolution.size(), 0);
       ASSERT_EQ(parallelSolution.size(), 0);
    }
}

TEST(GaussJordanTests, TestRandomMatrix) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    if (world.rank() == 0) {
        std::vector<std::vector<double>> A = generate_random_matrix(3, 3);
    }
    std::vector<double> b = {1, 2, 3};

    std::vector<double> parallelSolution = solve_linear_system_parallel(A, b);

    if (world.rank() == 0) {
        std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
        ASSERT_EQ(sequentialSolution, parallelSolution);
    }
}

TEST(GaussJordanTests, TestIdentityMatrix) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<std::vector<double>> A = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    std::vector<double> b = {1, 2, 3};

    std::vector<double> parallelSolution = solve_linear_system_parallel(A, b);

    if (world.rank() == 0) {
        std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
        ASSERT_EQ(sequentialSolution, parallelSolution);
    }
}

TEST(GaussJordanTests, TestLargeMatrix) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    const int matrixSize = 10;
    if (world.rank() == 0) {
        std::vector<std::vector<double>> A = generate_random_matrix(matrixSize, matrixSize);
    }
    std::vector<double> b(matrixSize, 1.0);

    std::vector<double> parallelSolution = solve_linear_system_parallel(A, b);

    if (world.rank() == 0) {
        std::vector<double> sequentialSolution = solve_linear_system_sequential(A, b);
        ASSERT_EQ(sequentialSolution, parallelSolution);
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
