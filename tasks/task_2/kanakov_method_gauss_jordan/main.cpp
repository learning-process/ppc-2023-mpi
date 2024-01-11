// Copyright 2023 Kanakov Roman
#include <gtest/gtest.h>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/serialization/vector.hpp>
#include "./method_gauss_jordan.h"

TEST(Gauss_Jordan_Method_MPI, wikipedia_system) {
    boost::mpi::communicator world;
    auto status = world.iprobe();
    std::vector<double> s = {1, 1, 1, 4, 2, 1, 9, 3, 1};
    std::vector<double> c = {0, 1, 3};
    std::vector<double> realSolution = {0.5, -0.5, 0};

    if (world.rank() == 0) {
        std::cout << "sequential method\n";
        auto l = gaussJordanMethodSequential(s, c);
        auto iterator = realSolution.begin();
        for (auto i : l) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
        std::cout << "parallel method \n";
    }
    auto result = gaussJordanMethodParallel(s, c);
    if (world.rank() == 0) {
        auto iterator = realSolution.begin();
        for (auto i : result) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
    }
    while ((status = world.iprobe()).has_value())
        world.recv(status.value().source(), status.value().tag());
}

TEST(Gauss_Jordan_Method_MPI, trivial_system) {
    boost::mpi::communicator world;
    auto status = world.iprobe();
    std::vector<double> s = {1, 2, 3, 2, 3, 4, 3, 4, 5};
    std::vector<double> c = {0, 0, 0};
    std::vector<double> realSolution = {0, 0, 0};

    if (world.rank() == 0) {
        std::cout << "sequential method\n";
        auto l = gaussJordanMethodSequential(s, c);
        auto iterator = realSolution.begin();
        for (auto i : l) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
        std::cout << "parallel method \n";
    }
    auto result = gaussJordanMethodParallel(s, c);
    if (world.rank() == 0) {
        auto iterator = realSolution.begin();
        for (auto i : result) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
    }
    while ((status = world.iprobe()).has_value())
        world.recv(status.value().source(), status.value().tag());
}

TEST(Gauss_Jordan_Method_MPI, upper_triangle_system) {
    boost::mpi::communicator world;
    auto status = world.iprobe();
    std::vector<double> s = {1, 2, 1, 0, 8, 4, 0, 0, 16};
    std::vector<double> c = {1, 2, 4};
    std::vector<double> realSolution = {0.5, 0.125, 0.25};

    if (world.rank() == 0) {
        std::cout << "sequential method\n";
        auto l = gaussJordanMethodSequential(s, c);
        auto iterator = realSolution.begin();
        for (auto i : l) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
        std::cout << "parallel method \n";
    }
    auto result = gaussJordanMethodParallel(s, c);
    if (world.rank() == 0) {
        auto iterator = realSolution.begin();
        for (auto i : result) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
    }
    while ((status = world.iprobe()).has_value())
        world.recv(status.value().source(), status.value().tag());
}

TEST(Gauss_Jordan_Method_MPI, big_random_system) {
    boost::mpi::communicator world;
    std::uniform_real_distribution r(-3.0, 5.0);
    std::random_device rd;
    auto status = world.iprobe();
    int matrixSize = 1000;
    std::vector<double> s(matrixSize * matrixSize);
    std::vector<double> c(matrixSize);
    if (world.rank() == 0) {
        for (int l : s)
            l = r(rd);
        for (int l : c)
            l = r(rd);
        boost::mpi::timer timer;
        gaussJordanMethodSequential(s, c);
        std::cout << "seq method time: " << timer.elapsed() << "\n";
    }
    boost::mpi::timer timer;
    gaussJordanMethodParallel(s, c);
    if (world.rank() == 0)
        std::cout << "par method time: " << timer.elapsed() << "\n";

    while ((status = world.iprobe()).has_value())
        world.recv(status.value().source(), status.value().tag());
}

TEST(Gauss_Jordan_Method_MPI, no_solutions) {
    boost::mpi::communicator world;
    auto status = world.iprobe();
    std::vector<double> s = {1, 2, 3, 1, 2, 3, 0, 0, 4};
    std::vector<double> c = {1, 2, 1};

    if (world.rank() == 0) {
        std::cout << "sequential method\n";
        ASSERT_ANY_THROW(gaussJordanMethodSequential(s, c));
        std::cout << "parallel method \n";
    }

    ASSERT_ANY_THROW(gaussJordanMethodParallel(s, c));

    while ((status = world.iprobe()).has_value())
        world.recv(status.value().source(), status.value().tag());
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
