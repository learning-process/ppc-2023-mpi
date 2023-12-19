// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>
#include <math.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/yurin_a_trapezoidal_rule/trapezoidal_rule.h"

TEST(Parallel_Operations_MPI, sin) {
    boost::mpi::communicator world;
    std::function function = [](double argument){return sin(argument);};
    double leftBound = 0;
    double rightBound = 3.14159265358979323846;
    double step = 3.14159265358979323846 / 2048.0;
    double expectedValue = 2;
    double tolerance = 1e-6;

    double parallelVersion = IntegrateWithTrapezoidRuleParallel(
            function,
            leftBound,
            rightBound,
            step);

    if (world.rank() == 0) {
        double sequentialVersion = IntegrateWithTrapezoidRuleSequential(
                function,
                leftBound,
                rightBound,
                step);

        ASSERT_NEAR(sequentialVersion, parallelVersion, tolerance);
        ASSERT_NEAR(expectedValue, parallelVersion, tolerance);
    }
}

TEST(Parallel_Operations_MPI, cos) {
    boost::mpi::communicator world;

    std::function function = [](double argument){return cos(argument);};
    double leftBound = 3.14159265358979323846;
    double rightBound = 3.14159265358979323846 / 5.0;
    double step = 3.14159265358979323846 / 2048.0;
    double expectedValue = sin(3.14159265358979323846 / 5.0);
    double tolerance = 1e-6;

    double parallelVersion = IntegrateWithTrapezoidRuleParallel(
            function,
            leftBound,
            rightBound,
            step);

    if (world.rank() == 0) {
        double sequentialVersion = IntegrateWithTrapezoidRuleSequential(
                function,
                leftBound,
                rightBound,
                step);

        ASSERT_NEAR(sequentialVersion, parallelVersion, tolerance);
        ASSERT_NEAR(expectedValue, parallelVersion, tolerance);
    }
}

TEST(Parallel_Operations_MPI, log10) {
    boost::mpi::communicator world;

    std::function function = [](double argument){return log10(argument);};
    double leftBound = 1;
    double rightBound = 10;
    double step = 1e-4;
    double expectedValue = 10 - 9 / log(10);
    double tolerance = 1e-6;

    double parallelVersion = IntegrateWithTrapezoidRuleParallel(
            function,
            leftBound,
            rightBound,
            step);

    if (world.rank() == 0) {
        double sequentialVersion = IntegrateWithTrapezoidRuleSequential(
                function,
                leftBound,
                rightBound,
                step);

        ASSERT_NEAR(sequentialVersion, parallelVersion, tolerance);
        ASSERT_NEAR(expectedValue, parallelVersion, tolerance);
    }
}

TEST(Parallel_Operations_MPI, left_bound_is_equal_right) {
    boost::mpi::communicator world;

    std::function function = [](double argument){return argument;};
    double leftBound = 1;
    double rightBound = 1;
    double step = 1e-4;
    double expectedValue = 0;
    double tolerance = 0;

    double parallelVersion = IntegrateWithTrapezoidRuleParallel(
            function,
            leftBound,
            rightBound,
            step);

    if (world.rank() == 0) {
        double sequentialVersion = IntegrateWithTrapezoidRuleSequential(
                function,
                leftBound,
                rightBound,
                step);

        ASSERT_NEAR(sequentialVersion, parallelVersion, tolerance);
        ASSERT_NEAR(expectedValue, parallelVersion, tolerance);
    }
}

TEST(Parallel_Operations_MPI, function_composition) {
    boost::mpi::communicator world;
    std::function function = [](double argument){
        return (sin(sqrt(argument)+3) * std::pow(2.7182818284590452354, sqrt(argument)) / sqrt(argument));};
    double leftBound = 1;
    double rightBound = 5;
    double step = 1e-4;
    double expectedValue = -12.501034;
    double tolerance = 1e-6;

    double parallelVersion = IntegrateWithTrapezoidRuleParallel(
            function,
            leftBound,
            rightBound,
            step);

    if (world.rank() == 0) {
        double sequentialVersion = IntegrateWithTrapezoidRuleSequential(
                function,
                leftBound,
                rightBound,
                step);

        ASSERT_NEAR(sequentialVersion, parallelVersion, tolerance);
        ASSERT_NEAR(expectedValue, parallelVersion, tolerance);
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
