// Copyright 2023 Kulikov Artem
#include <gtest/gtest.h>

#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/serialization/vector.hpp>
#include "./gauss_jordan_method.h"

bool doubleVectorsEqual(const std::vector<double>& a,
                        const std::vector<double>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }
    return true;
}

TEST(Gauss_Jordan_Method_MPI, wikipedia_system) {
    // Test with example system from
    // https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%93%D0%B0%D1%83%D1%81%D1%81%D0%B0_%E2%80%94_%D0%96%D0%BE%D1%80%D0%B4%D0%B0%D0%BD%D0%B0
    boost::mpi::communicator world;
    std::vector<double> s = {1, 1, 1, 4, 2, 1, 9, 3, 1};
    std::vector<double> c = {0, 1, 3};
    std::vector<double> expected = {0.5, -0.5, 0};

    auto parallelResult = gaussJordanMethodParallel(s, c);
    ASSERT_TRUE(doubleVectorsEqual(parallelResult, expected));
    if (world.rank() == 0) {
        auto sequentialResult = gaussJordanMethodSequential(s, c);
        ASSERT_TRUE(doubleVectorsEqual(sequentialResult, expected));
    }
}

TEST(Gauss_Jordan_Method_MPI, trivial_system) {
    boost::mpi::communicator world;
    std::vector<double> s = {1, 2, 3, 2, 3, 4, 3, 4, 5};
    std::vector<double> c = {0, 0, 0};
    std::vector<double> expected = {0, 0, 0};

    auto parallelResult = gaussJordanMethodParallel(s, c);
    ASSERT_TRUE(doubleVectorsEqual(parallelResult, expected));
    if (world.rank() == 0) {
        auto sequentialResult = gaussJordanMethodSequential(s, c);
        ASSERT_TRUE(doubleVectorsEqual(sequentialResult, expected));
    }
}

TEST(Gauss_Jordan_Method_MPI, upper_triangle_system) {
    boost::mpi::communicator world;
    std::vector<double> s = {1, 2, 1, 0, 8, 4, 0, 0, 16};
    std::vector<double> c = {1, 2, 4};
    std::vector<double> expected = {0.5, 0.125, 0.25};

    auto parallelResult = gaussJordanMethodParallel(s, c);
    ASSERT_TRUE(doubleVectorsEqual(parallelResult, expected));
    if (world.rank() == 0) {
        auto sequentialResult = gaussJordanMethodSequential(s, c);
        ASSERT_TRUE(doubleVectorsEqual(sequentialResult, expected));
    }
}

TEST(Gauss_Jordan_Method_MPI, big_random_system) {
    boost::mpi::communicator world;
    std::uniform_real_distribution r(-3.0, 5.0);
    std::random_device rd;
    int matrixSize = 100;
    std::vector<double> s(matrixSize * matrixSize);
    std::vector<double> c(matrixSize);

    if (world.rank() == 0) {
        for (int sequentialResult : s) {
            sequentialResult = r(rd);
        }
        for (int l : c) {
            l = r(rd);
        }
    }
    boost::mpi::broadcast(world, s.data(), s.size(), 0);
    boost::mpi::broadcast(world, c.data(), c.size(), 0);
    boost::mpi::timer timer;
    auto parallelResult = gaussJordanMethodParallel(s, c);
    if (world.rank() == 0) {
        std::cout << "par method time: " << timer.elapsed() << "\n";
        auto sequentialResult = gaussJordanMethodSequential(s, c);
        std::cout << "seq method time: " << timer.elapsed() << "\n";
        ASSERT_TRUE(doubleVectorsEqual(parallelResult, sequentialResult));
    }
}

TEST(Gauss_Jordan_Method_MPI, no_solutions) {
    boost::mpi::communicator world;
    auto status = world.iprobe();
    std::vector<double> s = {1, 2, 3, 1, 2, 3, 0, 0, 4};
    std::vector<double> c = {1, 2, 1};

    if (world.rank() == 0) {
        ASSERT_ANY_THROW(gaussJordanMethodSequential(s, c));
    }

    ASSERT_ANY_THROW(gaussJordanMethodParallel(s, c));
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
