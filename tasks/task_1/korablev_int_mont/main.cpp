// Copyright 2023 Korablev Nikita
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/environment.hpp>
#include <iostream>
#include "task_1/korablev_int_mont/integral.h"
#define ESTIMATE 0.0001

TEST(Integral, sin) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = 5, b = 11;
    const double real_var = 0.279236;

    double res = integral(a, b, N, fsin);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - res), ESTIMATE);
    }
}

TEST(Integral, cos) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = -10, b = -1.5;
    const double real_var = -1.54152;

    double res = integral(a, b, N, fcos);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - res), ESTIMATE);
    }
}

TEST(PARALLEL_Integral, sin) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = 5, b = 11;
    const double real_var = 0.279236;

    double res = parallel_integral(a, b, N, fsin);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - res), ESTIMATE);
    }
}

TEST(PARALLEL_Integral, cos) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = -10, b = -1.5;
    const double real_var = -1.54152;

    double res = parallel_integral(a, b, N, fcos);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - res), ESTIMATE);
    }
}

TEST(PARALLEL_Integral, sin_cos) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = -3, b = 4;
    const double real_var = 0.552835;

    double res = parallel_integral(a, b, N, f_two_sin_cos);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - res), ESTIMATE);
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
