// Copyright 2023 Vasyliev Ivan
#include <gtest/gtest.h>
#include <string>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./shell_sort_n_bathcer.h"


TEST(SHELL_SORT_N_BATCHER, Sort_Hand_4) {
    boost::mpi::communicator world;
    std::vector<int> array;
    if (world.rank() == 0) {
        array = { -2, 2, 1, -4 };
    }
    auto pSorted = ParallelSort(array, 4);
    if (world.rank() == 0) {
        ASSERT_EQ(std::vector<int>({-4, -2, 1, 2}), pSorted);
    }
}

TEST(SHELL_SORT_N_BATCHER, Sort_Hand_8) {
    boost::mpi::communicator world;
    std::vector<int> array;
    if (world.rank() == 0) {
        array = { 5, 3, 8, 4, 2, 6, 7, 1 };
    }
    auto pSorted = ParallelSort(array, 8);
    if (world.rank() == 0) {
        ASSERT_EQ(std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8}), pSorted);
    }
}

TEST(SHELL_SORT_N_BATCHER, Sort_Random_Positive) {
    boost::mpi::communicator world;
    int n = 24;
    std::vector<int> array;
    if (world.rank() == 0) {
        array = getRandomVector(n, 0, 100);
    }
    auto pSorted = ParallelSort(array, n);
    if (world.rank() == 0) {
        std::sort(array.begin(), array.end());
        ASSERT_EQ(array, pSorted);
    }
}

TEST(SHELL_SORT_N_BATCHER, Sort_Random_Negative) {
    boost::mpi::communicator world;
    int n = 24;
    std::vector<int> array;
    if (world.rank() == 0) {
        array = getRandomVector(n, -100, 0);
    }
    auto pSorted = ParallelSort(array, n);
    if (world.rank() == 0) {
        std::sort(array.begin(), array.end());
        ASSERT_EQ(array, pSorted);
    }
}

TEST(SHELL_SORT_N_BATCHER, Sort_Random) {
    boost::mpi::communicator world;
    int n = 24;
    std::vector<int> array;
    if (world.rank() == 0) {
        array = getRandomVector(n, -100, 100);
    }
    auto pSorted = ParallelSort(array, n);
    if (world.rank() == 0) {
        std::sort(array.begin(), array.end());
        ASSERT_EQ(array, pSorted);
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
