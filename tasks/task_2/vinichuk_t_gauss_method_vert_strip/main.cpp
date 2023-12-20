// Copyright 2023 Vinichuk Timofey
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_2/vinichuk_t_gauss_method_vert_strip/gauss_method.h"

TEST(Parallel_Operations_MPI, Test_MethodGauss1) {
    boost::mpi::communicator world;
    std::vector<double> matrix;

    if (world.rank() == 0) {
        matrix = { 2, 1, -1, 5,
                    4, -3, 2, -8,
                    1, 2, 3, 10 };
    }
    auto global = ParallelMethodGaussian(matrix, 3);

    if (world.rank() == 0) {
        auto reference = SeqMethodGaussian(matrix, 3);
        ASSERT_EQ(reference, global);
    }
}
TEST(Parallel_Operations_MPI, Test_MethodGauss2) {
    boost::mpi::communicator world;
    std::vector<double> matrix;

    if (world.rank() == 0) {
        matrix = { 2, 3, -1, 1, 10,
                    4, -2, 2, -1, -5,
                    3, 1, 5, 2, 15,
                    -1, 2, -4, 3, 8};
    }
    auto global = ParallelMethodGaussian(matrix, 4);

    if (world.rank() == 0) {
        auto reference = SeqMethodGaussian(matrix, 4);
        ASSERT_EQ(reference, global);
    }
}
TEST(Parallel_Operations_MPI, Test_MethodGauss3) {
    boost::mpi::communicator world;
    std::vector<double> matrix;

    if (world.rank() == 0) {
        matrix = { 2, 3, -1, 4, 0, 15,
                  -1, 5, 2, -1, 1, 8,
                   4, -2, 3, 2, -1, 10,
                   1, 1, -3, 1, 5, -3,
                  -2, 4, 1, -1, 2, 12};
    }
    auto global = ParallelMethodGaussian(matrix, 5);

    if (world.rank() == 0) {
        auto reference = SeqMethodGaussian(matrix, 5);
        ASSERT_EQ(reference, global);
    }
}
TEST(Parallel_Operations_MPI, Test_MethodGauss4) {
    boost::mpi::communicator world;
    std::vector<double> matrix;

    if (world.rank() == 0) {
        matrix = { 3, -2, 1, 4, -1, 2, 20,
                  -2, 5, 3, -1, 2, -1, 15,
                   4, 2, -1, 3, 1, -1, 10,
                   1, -3, 2, -4, 1, 5, 5,
                  -1, 4, -1, 1, 2, -3, -8,
                   2, -1, 3, -2, -1, 4, -12};
    }
    auto global = ParallelMethodGaussian(matrix, 6);

    if (world.rank() == 0) {
        auto reference = SeqMethodGaussian(matrix, 6);
        ASSERT_EQ(reference, global);
    }
}
TEST(Parallel_Operations_MPI, Test_MethodGauss5) {
    boost::mpi::communicator world;
    std::vector<double> matrix;

    if (world.rank() == 0) {
        matrix = { 2, -3, 4, 5, -1, 2, -3, 1, 2, -2, 15,
                   1, 5, -2, -3, 1, 4, 2, -2, 3, -1, 20,
                  -3, 1, 2, -1, 3, 1, 4, -2, 1, 5, 10,
                   4, -2, 3, 1, -1, 2, -1, 3, -2, 4, 5,
                  -1, 3, -1, 2, 1, -4, 5, 1, 2, -3, -8,
                   2, -1, 4, -2, -3, 1, 2, 3, 1, 4, -12,
                   1, 2, -3, 1, 4, 3, -1, 2, 2, -1, 7,
                  -2, 1, 3, -4, 1, -2, 1, 3, 2, 3, 14,
                   3, -4, 2, 1, 2, -1, -4, 1, 3, -2, 6,
                  -1, 1, -2, 3, -1, 2, 1, 5, -4, 1, 18};
    }
    auto global = ParallelMethodGaussian(matrix, 10);

    if (world.rank() == 0) {
        auto reference = SeqMethodGaussian(matrix, 10);
        ASSERT_EQ(reference, global);
    }
}
int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
