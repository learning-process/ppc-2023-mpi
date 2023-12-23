// Copyright 2023 Sadikov Damir
#include <gtest/gtest.h>
#include <vector>
#include "./iterative_method.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Sequential_Iterative_Method_MPI, Test_Seq_1) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        const int n = 3;
        std::vector<double> A{
             1.0,  0.3, -0.5,
            -0.2,  1.0,  0.3,
             0.8,  0.1,  1.0
        };
        std::vector<double> b{ 1.0, 2.0, 3.0 };
        std::vector<double> ans{ 1.34921, 1.74603, 1.74603 };
        std::vector<double> x = SequentialIter(A, b, n);
        ASSERT_TRUE(NormOfDifference(ans, x) < 1e-4);
    }
}

TEST(Parallel_Iterative_Method_MPI, Test_Par_1) {
    boost::mpi::communicator world;
    const int n = 3;
    std::vector<double> A;
    std::vector<double> b;
    if (world.rank() == 0) {
        A = {
             1.0,  0.3, -0.5,
            -0.2,  1.0,  0.3,
             0.8,  0.1,  1.0
        };
        b = { 1.0, 2.0, 3.0 };
    }
    std::vector<double> x = ParallelIter(A, b, n);
    if (world.rank() == 0) {
        std::vector<double> ans{ 1.34921, 1.74603, 1.74603 };
        ASSERT_TRUE(NormOfDifference(ans, x) < 1e-4);
    }
}

TEST(Parallel_Iterative_Method_MPI, Test_Par_2) {
    boost::mpi::communicator world;
    const int n = 4;
    std::vector<double> A;
    std::vector<double> b;
    if (world.rank() == 0) {
        A = {
             1.0,  0.1,  0.2, -0.3,
             0.1,  1.0,  0.3,  0.1,
            -0.1,  0.5,  1.0,  0.3,
            -0.2,  0.2,  0.3,  1.0
        };
        b = { 0.6, 1.9, -3.4, 2.3 };
    }
    std::vector<double> x = ParallelIter(A, b, n);
    if (world.rank() == 0) {
        std::vector<double> ans{ 2.66013, 2.98161, -5.81923, 3.98147 };
        ASSERT_TRUE(NormOfDifference(ans, x) < 1e-4);
    }
}

TEST(Parallel_Iterative_Method_MPI, Test_Par_2_with_zero_b) {
    boost::mpi::communicator world;
    const int n = 4;
    std::vector<double> A;
    std::vector<double> b;
    if (world.rank() == 0) {
        A = {
             1.0,  0.1,  0.2, -0.3,
             0.1,  1.0,  0.3,  0.1,
            -0.1,  0.5,  1.0,  0.3,
            -0.2,  0.2,  0.3,  1.0
        };
        b = { 0.0, 0.0, 0.0, 0.0 };
    }
    std::vector<double> x = ParallelIter(A, b, n);
    if (world.rank() == 0) {
        std::vector<double> ans{ 0.0, 0.0, 0.0, 0.0 };
        ASSERT_TRUE(NormOfDifference(ans, x) < 1e-4);
    }
}

TEST(Parallel_Iterative_Method_MPI, Test_Par_random_5) {
    boost::mpi::communicator world;
    const int n = 5;
    std::vector<double> A;
    std::vector<double> b;
    if (world.rank() == 0) {
        A = getRandomMatrix(n);
        b = getRandomVector(n);
    }
    std::vector<double> x = ParallelIter(A, b, n);
    if (world.rank() == 0) {
        std::vector<double> ans = SequentialIter(A, b, n);
        ASSERT_TRUE(NormOfDifference(ans, x) < 1e-4);
    }
}

TEST(Parallel_Iterative_Method_MPI, Test_Par_random_10) {
    boost::mpi::communicator world;
    const int n = 10;
    std::vector<double> A;
    std::vector<double> b;
    if (world.rank() == 0) {
        A = getRandomMatrix(n);
        b = getRandomVector(n);
    }
    std::vector<double> x = ParallelIter(A, b, n);
    if (world.rank() == 0) {
        std::vector<double> ans = SequentialIter(A, b, n);
        ASSERT_TRUE(NormOfDifference(ans, x) < 1e-4);
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
