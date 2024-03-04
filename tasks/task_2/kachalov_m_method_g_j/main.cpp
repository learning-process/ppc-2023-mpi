// Copyright 2023 Kachalov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/serialization/vector.hpp>
#include "./method_g_j.h"

TEST(Gauss_Jordan_Method_MPI, zero_system) {
    boost::mpi::communicator world;
    auto status = world.iprobe();
    std::vector<double> s = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<double> c = {0, 0, 0};
    std::vector<double> realSolution = {0, 0, 0};

    if (world.rank() == 0) {
        std::cout << "sequential method\n";
        auto l = sequential_method_g_j(s, c);
        auto iterator = realSolution.begin();
        for (auto i : l) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
        std::cout << "parallel method \n";
    }

    auto result = parallel_method_g_j(s, c);

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
        auto l = sequential_method_g_j(s, c);
        auto iterator = realSolution.begin();
        for (auto i : l) {
            ASSERT_NEAR(i, *iterator, std::numeric_limits<double>::epsilon());
            iterator++;
        }
        std::cout << "parallel method \n";
    }

    auto result = parallel_method_g_j(s, c);

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

TEST(Gauss_Jordan_Method_MPI, large_random_system) {
    boost::mpi::communicator world;
    std::uniform_real_distribution r(-3.0, 5.0);
    std::random_device rd;
    auto status = world.iprobe();
    int matrixSize = 100;
    std::vector<double> s(matrixSize * matrixSize);
    std::vector<double> c(matrixSize);

    if (world.rank() == 0) {
        for (int i = 0; i < matrixSize * matrixSize; ++i)
            s[i] = r(rd);
        for (int i = 0; i < matrixSize; ++i)
            c[i] = r(rd);

        boost::mpi::timer timer;
        sequential_method_g_j(s, c);
        std::cout << "seq method time: " << timer.elapsed() << "\n";
    }

    boost::mpi::timer timer;
    parallel_method_g_j(s, c);

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
        ASSERT_ANY_THROW(sequential_method_g_j(s, c));
        std::cout << "parallel method \n";
    }

    ASSERT_ANY_THROW(parallel_method_g_j(s, c));

    while ((status = world.iprobe()).has_value())
        world.recv(status.value().source(), status.value().tag());
}

TEST(Gauss_Jordan_Method_MPI, example_system) {
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
