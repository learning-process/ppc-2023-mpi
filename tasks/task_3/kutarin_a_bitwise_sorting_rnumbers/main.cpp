// Copyright 2023 Kutarin Aleksandr
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "task_3/kutarin_a_bitwise_sorting_rnumbers/radix_sort_batcher.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Sequential_Radix_Sort_MPI, Test_Seq) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        std::vector<double> t = getRandomVector(16);
        std::vector<double> res = t;
        sort(res.begin(), res.end());
        t = iterativeRadixSort(t);
        ASSERT_EQ(res, t);
    }
}

TEST(Parallel_Radix_Sort_Batcher_Merge_MPI, Test_Par_manual) {
    const int n = 8;
    boost::mpi::communicator world;
    std::vector<double> v;
    std::vector<double> true_ans;
    if (world.rank() == 0) {
        v = { -6, 7, 5, -10, 6, 3, -7, -5 };
        true_ans = { -10, -7, -6, -5, 3, 5, 6, 7 };
    }
    v = parallelBatcherRadixSort(v, n);
    if (world.rank() == 0) {
        ASSERT_EQ(v, true_ans);
    }
}

TEST(Parallel_Radix_Sort_Batcher_Merge_MPI, Test_Par_4) {
    const int n = 4;
    boost::mpi::communicator world;
    std::vector<double> v;
    std::vector<double> true_ans;
    if (world.rank() == 0) {
        v = getRandomVector(n);
        true_ans = v;
    }
    v = parallelBatcherRadixSort(v, n);
    if (world.rank() == 0) {
        std::sort(true_ans.begin(), true_ans.end());
        ASSERT_EQ(v, true_ans);
    }
}

TEST(Parallel_Radix_Sort_Batcher_Merge_MPI, Test_Par_32) {
    const int n = 32;
    boost::mpi::communicator world;
    std::vector<double> v;
    std::vector<double> true_ans;
    if (world.rank() == 0) {
        v = getRandomVector(n);
        true_ans = v;
    }
    v = parallelBatcherRadixSort(v, n);
    if (world.rank() == 0) {
        std::sort(true_ans.begin(), true_ans.end());
        ASSERT_EQ(v, true_ans);
    }
}

TEST(Parallel_Radix_Sort_Batcher_Merge_MPI, Test_Par_128) {
    const int n = 128;
    boost::mpi::communicator world;
    std::vector<double> v;
    std::vector<double> true_ans;
    if (world.rank() == 0) {
        v = getRandomVector(n);
        true_ans = v;
    }
    v = parallelBatcherRadixSort(v, n);
    if (world.rank() == 0) {
        std::sort(true_ans.begin(), true_ans.end());
        ASSERT_EQ(v, true_ans);
    }
}

TEST(Parallel_Radix_Sort_Batcher_Merge_MPI, Test_Par_1024) {
    const int n = 1024;
    boost::mpi::communicator world;
    std::vector<double> v;
    std::vector<double> true_ans;
    if (world.rank() == 0) {
        v = getRandomVector(n);
        true_ans = v;
    }
    v = parallelBatcherRadixSort(v, n);
    if (world.rank() == 0) {
        std::sort(true_ans.begin(), true_ans.end());
        ASSERT_EQ(v, true_ans);
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
