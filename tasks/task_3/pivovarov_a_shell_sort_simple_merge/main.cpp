// Copyright 2023 Pivovarov Alexey

#include <gtest/gtest.h>
#include "./shell_sort_simple_merge.h"


TEST(ShellSortTest, EmptyArray) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<int> parallel_arr{};
    std::vector<int> sequential_arr{};

    shell_sort_par(parallel_arr, world);

    if (world.rank() == 0) {
        shell_sort_seq(sequential_arr);
        ASSERT_EQ(parallel_arr, sequential_arr);
    }
}

TEST(ShellSortTest, SortedArray) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<int> parallel_arr{1, 2, 3, 4, 5};
    std::vector<int> sequential_arr{1, 2, 3, 4, 5};

    shell_sort_par(parallel_arr, world);

    if (world.rank() == 0) {
        shell_sort_seq(sequential_arr);
        ASSERT_EQ(parallel_arr, sequential_arr);
    }
}

TEST(ShellSortTest, ReverseSortedArray) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<int> parallel_arr{5, 4, 3, 2, 1};
    std::vector<int> sequential_arr{5, 4, 3, 2, 1};

    shell_sort_par(parallel_arr, world);

    if (world.rank() == 0) {
        shell_sort_seq(sequential_arr);
        ASSERT_EQ(parallel_arr, sequential_arr);
    }
}

TEST(ShellSortTest, RandomArray) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<int> arr(10);
    arr = generate_random_array(10, 1, 9);

    std::vector<int> parallel_arr = arr;
    std::vector<int> sequential_arr = arr;

    shell_sort_par(parallel_arr, world);

    if (world.rank() == 0) {
        shell_sort_seq(sequential_arr);
        ASSERT_EQ(parallel_arr, sequential_arr);
    }
}

TEST(ShellSortTest, DuplicateValuesArray) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::vector<int> parallel_arr{2, 1, 2, 1, 3, 2, 3, 1, 3, 2};
    std::vector<int> sequential_arr{2, 1, 2, 1, 3, 2, 3, 1, 3, 2};

    shell_sort_par(parallel_arr, world);

    if (world.rank() == 0) {
        shell_sort_seq(sequential_arr);
        ASSERT_EQ(parallel_arr, sequential_arr);
    }
}

int main(int argc, char **argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
