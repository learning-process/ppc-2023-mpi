// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_3/vinichuk_t_radix_sort_usual/radix_sort_usual.h"

TEST(Parallel_Operations_MPI, Test_RadixSortUsual1) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const int count_size_vector = 20;

    if (world.rank() == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    auto global = ParallelRadixSortUsualMerge(global_vec);

    if (world.rank() == 0) {
        auto reference = SeqRadixSortUsualMerge(global_vec);
        ASSERT_EQ(reference, global);
    }
}


TEST(Parallel_Operations_MPI, Test_RadixSortUsual2) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const int count_size_vector = 50;

    if (world.rank() == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    auto global = ParallelRadixSortUsualMerge(global_vec);

    if (world.rank() == 0) {
        auto reference = SeqRadixSortUsualMerge(global_vec);
        ASSERT_EQ(reference, global);
    }
}

TEST(Parallel_Operations_MPI, Test_RadixSortUsual3) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const int count_size_vector = 100;

    if (world.rank() == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    auto global = ParallelRadixSortUsualMerge(global_vec);

    if (world.rank() == 0) {
        auto reference = SeqRadixSortUsualMerge(global_vec);
        ASSERT_EQ(reference, global);
    }
}


TEST(Parallel_Operations_MPI, Test_RadixSortUsual4) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const int count_size_vector = 200;

    if (world.rank() == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    auto global = ParallelRadixSortUsualMerge(global_vec);

    if (world.rank() == 0) {
        auto reference = SeqRadixSortUsualMerge(global_vec);
        ASSERT_EQ(reference, global);
    }
}

TEST(Parallel_Operations_MPI, Test_RadixSortUsual5) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const int count_size_vector = 500;

    if (world.rank() == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    auto global = ParallelRadixSortUsualMerge(global_vec);

    if (world.rank() == 0) {
        auto reference = SeqRadixSortUsualMerge(global_vec);
        ASSERT_EQ(reference, global);
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
