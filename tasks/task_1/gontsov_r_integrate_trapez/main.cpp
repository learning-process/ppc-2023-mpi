// Copyright 2023 Gontsov Roman

#include <gtest/gtest.h>
#include <mpi.h>
#include <cmath>
#include <algorithm>
#include <functional>
#include "./integrate_trapez.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

TEST(Parallel_Operations_MPI, Test_Square) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return x * x; };
    double i2 = getParallelIntegration(f, 1, 2, 3);
    if (rank == 0) {
        double i1 = getSequentialIntegration(f, 1, 2, 3);
        ASSERT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Parallel_Operations_MPI, Test_Sin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return sin(x); };
    double i2 = getParallelIntegration(f, -1, 0, 17);
    if (rank == 0) {
        double i1 = getSequentialIntegration(f, -1, 0, 17);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Parallel_Operations_MPI, Test_Polyn) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return x * x - 3 * x + 7; };
    double i2 = getParallelIntegration(f, -2, 3, 7);
    if (rank == 0) {
        double i1 = getSequentialIntegration(f, -2, 3, 7);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Parallel_Operations_MPI, Test_Cube) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) { return x * x * x; };
    double i2 = getParallelIntegration(f, -30, 30, 30);
    if (rank == 0) {
        double i1 = getSequentialIntegration(f, -30, 30, 30);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Parallel_Operations_MPI, Test_Cube_One_Interval) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) { return x * x * x; };
    double i2 = getParallelIntegration(f, -10, 10, 1);
    if (rank == 0) {
        double i1 = getSequentialIntegration(f, -10, 10, 1);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator communicator;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (communicator.rank()) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
