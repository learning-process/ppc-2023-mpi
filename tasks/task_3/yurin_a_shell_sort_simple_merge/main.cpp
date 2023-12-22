// Copyright 2023 Yurin Andrey
#include <gtest/gtest.h>
#include "./shell_sort_simple_merge.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Operations_MPI, Test_IsSorted) {
    boost::mpi::communicator world;
    std::vector<int> sortedArray = {1, 2, 3, 6, 10, 17};
    std::vector<int> notSortedArray = {1, 2, 6, 3, 10, 17, 2};

    if (world.rank() == 0) {
        ASSERT_TRUE(IsSorted(sortedArray));
        ASSERT_FALSE(IsSorted(notSortedArray));
    }
}

TEST(Parallel_Operations_MPI, One_Element_In_Array) {
    boost::mpi::communicator world;
    std::vector<int> array = {1};

    std::vector<int> sortedArrayByFirstParallelVersion = ShellSortParallel(array);
    std::vector<int> sortedArrayBySecondParallelVersion = ShellSortParallel(array, true);

    if (world.rank() == 0) {
        std::vector<int> sortedArrayBySequentialVersion = ShellSortSequential(array);
        ASSERT_TRUE(IsSorted(sortedArrayBySequentialVersion));
        ASSERT_TRUE(IsSorted(sortedArrayByFirstParallelVersion));
        ASSERT_TRUE(IsSorted(sortedArrayBySecondParallelVersion));
    }
}

TEST(Parallel_Operations_MPI, Two_Element_In_Array) {
    boost::mpi::communicator world;
    std::vector<int> array = {8, 5};

    std::vector<int> sortedArrayByFirstParallelVersion = ShellSortParallel(array);
    std::vector<int> sortedArrayBySecondParallelVersion = ShellSortParallel(array, true);

    if (world.rank() == 0) {
        std::vector<int> sortedArrayBySequentialVersion = ShellSortSequential(array);
        ASSERT_TRUE(IsSorted(sortedArrayBySequentialVersion));
        ASSERT_TRUE(IsSorted(sortedArrayByFirstParallelVersion));
        ASSERT_TRUE(IsSorted(sortedArrayBySecondParallelVersion));
    }
}

TEST(Parallel_Operations_MPI, Random_Array) {
    boost::mpi::communicator world;
    std::vector<int> array(10000);
    if (world.rank() == 0) {
        array = GetRandomVector(array.size(), 0, 10000);
    }
    std::vector<int> sortedArrayByFirstParallelVersion = ShellSortParallel(array);
    std::vector<int> sortedArrayBySecondParallelVersion = ShellSortParallel(array, true);

    if (world.rank() == 0) {
        std::vector<int> sortedArrayBySequentialVersion = ShellSortSequential(array);
        ASSERT_TRUE(IsSorted(sortedArrayBySequentialVersion));
        ASSERT_TRUE(IsSorted(sortedArrayByFirstParallelVersion));
        ASSERT_TRUE(IsSorted(sortedArrayBySecondParallelVersion));
    }
}

TEST(Parallel_Operations_MPI, Sorted_Array) {
    boost::mpi::communicator world;
    std::vector<int> array;
    for (int i = 0; i < 10000; ++i) {
        array.push_back(i);
    }
    std::vector<int> sortedArrayByFirstParallelVersion = ShellSortParallel(array);
    std::vector<int> sortedArrayBySecondParallelVersion = ShellSortParallel(array, true);

    if (world.rank() == 0) {
        std::vector<int> sortedArrayBySequentialVersion = ShellSortSequential(array);
        ASSERT_TRUE(IsSorted(sortedArrayBySequentialVersion));
        ASSERT_TRUE(IsSorted(sortedArrayByFirstParallelVersion));
        ASSERT_TRUE(IsSorted(sortedArrayBySecondParallelVersion));
    }
}

TEST(Parallel_Operations_MPI, Reverse_Sorted_Array) {
    boost::mpi::communicator world;
    std::vector<int> array;
    for (int i = 0; i < 10000; ++i) {
        array.push_back(100000 - i);
    }
    std::vector<int> sortedArrayByFirstParallelVersion = ShellSortParallel(array);
    std::vector<int> sortedArrayBySecondParallelVersion = ShellSortParallel(array, true);

    if (world.rank() == 0) {
        std::vector<int> sortedArrayBySequentialVersion = ShellSortSequential(array);
        ASSERT_TRUE(IsSorted(sortedArrayBySequentialVersion));
        ASSERT_TRUE(IsSorted(sortedArrayByFirstParallelVersion));
        ASSERT_TRUE(IsSorted(sortedArrayBySecondParallelVersion));
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
