// Copyright 2023 Dostavalov Semen
#include <math.h>
#include <gtest/gtest.h>
#include <boost/core/invoke_swap.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/dostavalov_s_monte_carlo_method/monte_carlo_method.h"

#define count 1000000
#define err 0.1

double f1(double x) {
    return (pow(x, 2));
}

double f2(double x) {
    return (1 / log(x));
}

double f3(double x) {
    return (exp(x) / x);
}

double f4(double x) {
    return (log(x) + (5 * x));
}

double f5(double x) {
    return (exp(x) * pow(x, 2));
}


TEST(MPI_TESTS, Test_1) {
    bool flag = false;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int low = 0;
    int high = 1;

    double res = monteCarlo(low, high, count, &f1);
    double corRes = 0.33;

    if (rank == 0) {
        if (abs(res - corRes) <= err)
            flag = true;
        ASSERT_EQ(flag, true);
}
}

TEST(MPI_TESTS, Test_hard_log) {
    bool flag = false;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int low = 2;
    int high = 3;

    double res = monteCarlo(low, high, count, &f2);
    double corRes = 1.1;
    if (rank == 0) {
        if (abs(res - corRes) <= err)
            flag = true;
        ASSERT_EQ(flag, true);
    }
}

TEST(MPI_TESTS, Test_hard_exp) {
    bool flag = false;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int low = 1;
    int high = 2;

    double res = monteCarlo(low, high, count, &f3);
    double corRes = 3.05;
    if (rank == 0) {
        if (abs(res - corRes) <= err)
            flag = true;
        ASSERT_EQ(flag, true);
    }
}
TEST(MPI_TESTS, Test_log) {
    bool flag = false;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int low = 0;
    int high = 1;

    double res = monteCarlo(low, high, count, &f4);
    double corRes = 1.9;
    if (rank == 0) {
        if (abs(res - corRes) <= err)
            flag = true;
        ASSERT_EQ(flag, true);
    }
}

TEST(MPI_TESTS, Test_exp) {
    bool flag = false;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);;

    int low = 0;
    int high = 1;

    double res = monteCarlo(low, high, count, &f5);
    double corRes = 0.71;
    if (rank == 0) {
        if (abs(res - corRes) <= err)
            flag = true;
        ASSERT_EQ(flag, true);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners
        = ::testing::UnitTest::GetInstance()->listeners();

    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}

