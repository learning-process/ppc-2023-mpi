// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include "./rect_integration.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <cmath>

TEST(Parallel_Rect_Integration_MPI, Test_x3_plus_x2_plus_5) {
    boost::mpi::communicator world;
    auto func = [](double x) { return x*x*x+x*x+5.0;};

    const uint32_t count_partitions = 30000;
    const double eps = 10.0 / count_partitions;  
    const double a = 1.0;
    const double b = 2.0;
    double global_sum = getParallelIntegral(func, a, b, count_partitions);

    if (world.rank() == 0) {
        double reference_sum = 133.0/12.0;
        ASSERT_NEAR(reference_sum, global_sum, eps);
    }
}

TEST(Parallel_Rect_Integration_MPI, Test_exp_sin_x) {
    boost::mpi::communicator world;
    auto func = [](double x) { return std::exp(std::sin(x));};

    const uint32_t count_partitions = 30000;
    const double eps = 10.0 / count_partitions;  
    const double a = 1.0;
    const double b = 2.0;
    double global_sum = getParallelIntegral(func, a, b, count_partitions);

    if (world.rank() == 0) {
        double reference_sum = getSequentialIntegral(func, a, b, count_partitions);
        ASSERT_NEAR(reference_sum, global_sum, eps);
    }
}

TEST(Parallel_Rect_Integration_MPI, Test_cos_cos_x) {
    boost::mpi::communicator world;
    auto func = [](double x) { return std::cos(std::cos(x));};

    const uint32_t count_partitions = 30000;
    const double eps = 10.0 / count_partitions;  
    const double a = 1.0;
    const double b = 2.0;
    double global_sum = getParallelIntegral(func, a, b, count_partitions);

    if (world.rank() == 0) {
        double reference_sum = getSequentialIntegral(func, a, b, count_partitions);
        ASSERT_NEAR(reference_sum, global_sum, eps);
    }
}

TEST(Parallel_Rect_Integration_MPI, Test_identical_zero) {
    boost::mpi::communicator world;
    auto func = [](double x) { return 0.0;};

    const uint32_t count_partitions = 30000;
    const double eps = 10.0 / count_partitions;  
    const double a = 1.0;
    const double b = 2.0;
    double global_sum = getParallelIntegral(func, a, b, count_partitions);

    if (world.rank() == 0) {
        double reference_sum = 0.0;
        ASSERT_NEAR(reference_sum, global_sum, eps);
    }
}

TEST(Parallel_Rect_Integration_MPI, Test_odd_function_symmetric_segment) {
    boost::mpi::communicator world;
    auto func = [](double x) { return x*x*x*x*x;};

    const uint32_t count_partitions = 30000;
    const double eps = 10.0 / count_partitions;  
    const double a = -1.0;
    const double b = 1.0;
    double global_sum = getParallelIntegral(func, a, b, count_partitions);

    if (world.rank() == 0) {
        double reference_sum = 0.0;
        ASSERT_NEAR(reference_sum, global_sum, eps);
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
