// Copyright 2023 Kachalov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./method_g_j.h"

TEST(MethodGJTests, TestEmptyMatrixSequential) {
    boost::mpi::communicator world;
    std::vector<double> matrix;
    std::vector<double> constants;
    if (world.rank() == 0) {
        ASSERT_THROW(sequential_method_g_j(matrix, constants), std::invalid_argument);
    }
}

TEST(MethodGJTests, TestEmptyMatrixParallel) {
    boost::mpi::communicator world;
    std::vector<double> matrix;
    std::vector<double> constants;

    if (world.rank() == 0) {
        ASSERT_THROW(parallel_method_g_j(matrix, constants), std::invalid_argument);
    }
}

TEST(MethodGJTests, TestCorrectSolutionSequential) {
    boost::mpi::communicator world;
    std::vector<double> matrix = {2, 1, -1, -3, -1, 2, 1, 1, 1};
    std::vector<double> constants = {8, -11, -3};

    if (world.rank() == 0) {
        std::vector<double> result = sequential_method_g_j(matrix, constants);
        std::vector<double> expected = {2, -1, 3};
        ASSERT_EQ(result, expected);
    }
}

TEST(MethodGJTests, TestCorrectSolutionParallel) {
    boost::mpi::communicator world;
    std::vector<double> matrix = {2, 1, -1, -3, -1, 2, 1, 1, 1};
    std::vector<double> constants = {8, -11, -3};

    if (world.rank() == 0) {
        std::vector<double> result = parallel_method_g_j(matrix, constants);
        std::vector<double> expected = {2, -1, 3};
        ASSERT_EQ(result, expected);
    }
}

TEST(MethodGJTests, TestZeroDeterminantSequential) {
    boost::mpi::communicator world;
    std::vector<double> matrix = {1, 2, 3, 2, 4, 6, 3, 6, 9};
    std::vector<double> constants = {3, 6, 9};

    if (world.rank() == 0) {
        ASSERT_THROW(sequential_method_g_j(matrix, constants), std::invalid_argument);
    }
}

TEST(MethodGJTests, TestZeroDeterminantParallel) {
    boost::mpi::communicator world;
    std::vector<double> matrix = {1, 2, 3, 2, 4, 6, 3, 6, 9};
    std::vector<double> constants = {3, 6, 9};

    if (world.rank() == 0) {
        ASSERT_THROW(parallel_method_g_j(matrix, constants), std::invalid_argument);
    }
}

TEST(MethodGJTests, TestRandomMatrixSequential) {
    boost::mpi::communicator world;
    std::vector<double> constants = {1, 2, 3};

    if (world.rank() == 0) {
        std::vector<double> matrix = RandomMatrix(3, &gen);
        ASSERT_NO_THROW(sequential_method_g_j(matrix, constants));
    }
}

TEST(MethodGJTests, TestRandomMatrixParallel) {
    boost::mpi::communicator world;
    std::vector<double> constants = {1, 2, 3};

    if (world.rank() == 0) {
        std::vector<double> matrix = RandomMatrix(3, &gen);
        ASSERT_NO_THROW(parallel_method_g_j(matrix, constants));
    }
}

TEST(MethodGJTests, TestIncorrectMatrixSequential) {
    boost::mpi::communicator world;
    std::vector<double> matrix = {1, 2, 3, 2, 4, 6};
    std::vector<double> constants = {3, 6};

    if (world.rank() == 0) {
        ASSERT_THROW(sequential_method_g_j(matrix, constants), std::invalid_argument);
    }
}

TEST(MethodGJTests, TestIncorrectMatrixParallel) {
    boost::mpi::communicator world;
    std::vector<double> matrix = {1, 2, 3, 2, 4, 6};
    std::vector<double> constants = {3, 6};

    if (world.rank() == 0) {
        ASSERT_THROW(parallel_method_g_j(matrix, constants), std::invalid_argument);
    }
}

TEST(MethodGJTests, TestLargeMatrixSequential) {
    boost::mpi::communicator world;
    std::vector<double> constants(100, 1);

    if (world.rank() == 0) {
        std::vector<double> matrix = RandomMatrix(100, &gen);
        ASSERT_NO_THROW(sequential_method_g_j(matrix, constants));
    }
}

TEST(MethodGJTests, TestLargeMatrixParallel) {
    boost::mpi::communicator world;
    std::vector<double> constants(100, 1);

    if (world.rank() == 0) {
        std::vector<double> matrix = RandomMatrix(100, &gen);
        ASSERT_NO_THROW(parallel_method_g_j(matrix, constants));
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
