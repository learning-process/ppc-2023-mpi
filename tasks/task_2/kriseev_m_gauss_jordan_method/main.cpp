// Copyright 2023 Kulikov Artem
#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./gauss_jordan_method.h"

TEST(Gauss_Jordan_Method_MPI, wikipedia_system) {
    // Test with example system from
    // https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%93%D0%B0%D1%83%D1%81%D1%81%D0%B0_%E2%80%94_%D0%96%D0%BE%D1%80%D0%B4%D0%B0%D0%BD%D0%B0
    boost::mpi::communicator world;
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
}

TEST(Gauss_Jordan_Method_MPI, trivial_system) {
    boost::mpi::communicator world;
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
}

/*TEST(Gauss_Jordan_Method_MPI, upper_triangle_system) {

}

TEST(Gauss_Jordan_Method_MPI, big_random_system) {

}

TEST(Gauss_Jordan_Method_MPI, big_random_system_performance)
{

}*/


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
