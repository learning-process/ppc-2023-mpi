// Copyright 2023 dmitry-vnn
#include <gtest/gtest.h>
#include <cmath>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/vanushkin_d_rect_integration/rectangle_integration.h"

TEST(rectangle_integration_parallel, identity_function) {
    boost::mpi::communicator world;

    IntegrateFunction f = std::identity();

    double lowerBound = -10;
    double upperBound = 10;
    double step = 1;

    double parallelVersion = RectangleIntegrateParallel(
            f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        double sequentialVersion =
                RectangleIntegrateSequential(f, lowerBound, upperBound, step);
        ASSERT_EQ(sequentialVersion, parallelVersion);
    }
}

TEST(rectangle_integration_parallel, identity_function_small_step) {
    boost::mpi::communicator world;

    IntegrateFunction f = std::identity();

    double lowerBound = -10;
    double upperBound = 10;
    double step = 0.00001;

    double parallelVersion =
            RectangleIntegrateParallel(f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        double sequentialVersion =
                RectangleIntegrateSequential(f, lowerBound, upperBound, step);
        ASSERT_EQ(sequentialVersion, parallelVersion);
    }
}

TEST(rectangle_integration_parallel, square_function) {
    boost::mpi::communicator world;

    IntegrateFunction f = [](auto x) -> auto {
        return x * x;
    };

    double lowerBound = -100;
    double upperBound = 100;
    double step = 0.00001;

    double parallelVersion =
            RectangleIntegrateParallel(f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        double sequentialVersion =
                RectangleIntegrateSequential(f, lowerBound, upperBound, step);
        ASSERT_EQ(sequentialVersion, parallelVersion);
    }
}

TEST(rectangle_integration_parallel, sinx) {
    boost::mpi::communicator world;

    IntegrateFunction f = [](auto x) -> auto {
        return std::sin(x);
    };

    double lowerBound = 0;
    double upperBound = atan(1) * 2;

    double step = 0.000001;

    double parallelVersion =
            RectangleIntegrateParallel(f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        double sequentialVersion =
                RectangleIntegrateSequential(f, lowerBound, upperBound, step);
        ASSERT_EQ(sequentialVersion, parallelVersion);
    }
}

TEST(rectangle_integration_parallel, hiperbola_function) {
    boost::mpi::communicator world;

    IntegrateFunction f = [](auto x) -> auto {
        return 1 / x;
    };

    double lowerBound = 0.25;
    double upperBound = 50;

    double step = 0.0001;

    double parallelVersion =
            RectangleIntegrateParallel(f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        double sequentialVersion =
                RectangleIntegrateSequential(f, lowerBound, upperBound, step);
        ASSERT_EQ(sequentialVersion, parallelVersion);
    }
}

TEST(rectangle_integration_parallel, too_little_bounds_with_overflow_step) {
    boost::mpi::communicator world;

    IntegrateFunction f = [](auto x) -> auto {
        return 1 / x;
    };

    double lowerBound = 0.009;
    double upperBound = 0.010;

    double step = 1;

    double parallelVersion =
            RectangleIntegrateParallel(f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        double sequentialVersion =
                RectangleIntegrateSequential(f, lowerBound, upperBound, step);
        ASSERT_EQ(sequentialVersion, parallelVersion);
    }
}

TEST(rectangle_integration_parallel, constant_function) {
    boost::mpi::communicator world;

    IntegrateFunction f = [](auto x) -> auto {
        return 10;
    };

    double lowerBound = -100;
    double upperBound = 100;

    double step = 1;


    double parallelVersion =
            RectangleIntegrateParallel(f, lowerBound, upperBound, step);

    if (world.rank() == 0) {
        ASSERT_EQ(2000, parallelVersion);
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
