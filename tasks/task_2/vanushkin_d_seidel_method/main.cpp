// Copyright 2023 Vanushkin Dmitry
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_2/vanushkin_d_seidel_method/seidel_method.h"


TEST(seidel_method, sequential_test) {

    DoubleMatrix a = {
            {5, 0, 0},
            {0, 4, 0},
            {0, 7, 14},
    };

    DoubleVector b = {10, 2, 28};

    auto result = SequentialSeidelMethod(a, b, 0.001);
    DoubleVector correctSolution = {2, 0.5, 1.75};

    ASSERT_TRUE(norm(result, correctSolution) <= 0.001);

}

TEST(seidel_method, parallel_3x3) {

    boost::mpi::communicator world;

    DoubleMatrix a = {
            {5, 0, 0},
            {0, 4, 0},
            {0, 0, 14},
    };

    DoubleVector b = {10, 2, 3.5};

    double eps = 0.001;

    auto parallelResult = ParallelSeidelMethod(a, b, eps);

    if (world.rank() == 0) {

        auto sequentialResult = SequentialSeidelMethod(a, b, eps);
        ASSERT_EQ(parallelResult, sequentialResult);
    }

}


TEST(seidel_method, parallel_7x7) {

    boost::mpi::communicator world;

    DoubleMatrix a = {
            {80, 3, 5, 2, 4, 7, 1},
            {9, 45, 1, 4, 8, 3, 2},
            {3, 7, 51, 3, 5, 2, 6},
            {0, 6, 3, 37, 1, 4, 3},
            {1, 8, 2, 11, 35, 1, 4},
            {5, 1, 6, 2, 5, 41, 5},
            {7, 1, 11, 4, 5, 1, 26},
    };

    DoubleVector b = {
        1, 5, 6, 3, 8, 9, 2
    };

    double eps = 0.001;

    auto parallelResult = ParallelSeidelMethod(a, b, eps);

    if (world.rank() == 0) {

        auto sequentialResult = SequentialSeidelMethod(a, b, eps);
        ASSERT_EQ(parallelResult, sequentialResult);
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
