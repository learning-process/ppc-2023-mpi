// Copyright 2023 Ivanov Nikita
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include "task_1/ivanov_int_tr/int_tr.h"
#define ESTIMATE 0.00001

TEST(Parallel_Integral, sin) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = 1, b = 11;
    const double real_var = 0.535876607880088;

    double global_sum = getParallelOperations(a, b, N, sin_f);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - global_sum), ESTIMATE);
    }
}

TEST(Parallel_Integral, hard_fun) {
    boost::mpi::communicator world;
    const int N = 100000;
    const double a = -0.75;
    const double b = 1;
    const double real_var = -2.27207793864214;


    double global_sum = getParallelOperations(a, b, N, hardfn_f);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - global_sum), ESTIMATE);
    }
}

TEST(Parallel_Integral, sin_2) {
    boost::mpi::communicator world;
    const int N = 1000000;
    const double a = -1;
    const double b = 1.56;
    const double real_var = 90.77507881491248;

    double global_sum = getParallelOperations(a, b, N, sin2_f);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - global_sum), ESTIMATE);
    }
}

TEST(Parallel_Integral, hard_fun_2) {
    boost::mpi::communicator world;
    const int N = 1000000;
    const double a = -5;
    const double b = -2.2;
    const double real_var = 0.00201953418482;

    double global_sum = getParallelOperations(a, b, N, hardfn2_f);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - global_sum), ESTIMATE);
    }
}

TEST(Parallel_Integral, sin_cos) {
    boost::mpi::communicator world;
    const int N = 1000000;
    const double a = 16.54;
    const double b = 20;
    const double real_var = -0.124428146857;

    double global_sum = getParallelOperations(a, b, N, sin_cos_f);

    if (world.rank() == 0) {
        ASSERT_LT(std::abs(real_var - global_sum), ESTIMATE);
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
