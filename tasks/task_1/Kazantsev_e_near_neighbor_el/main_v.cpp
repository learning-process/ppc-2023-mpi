// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "../tasks/task_1/Kazantsev_e_near_neighbor_el/near_neighbor_el.h"

TEST(Parallel_Operations_MPI, Test_even_count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 100;
    if (rank == 0) {
        global_vec = getRandVec(count_size_vector);
    }
    int globalMin = parallelCount(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_min = SequentialCount(global_vec);
        ASSERT_EQ(globalMin, reference_min);
    }
}

TEST(Parallel_Operations_MPI, Test_even_count_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    if (rank == 0) {
        global_vec = getRandVec(count_size_vector);
    }
    int globalMin = parallelCount(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_min = SequentialCount(global_vec);
        ASSERT_EQ(globalMin, reference_min);
    }
}

TEST(Parallel_Operations_MPI, Test_uneven_count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 111;
    if (rank == 0) {
        global_vec = getRandVec(count_size_vector);
    }
    int globalMin = parallelCount(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_min = SequentialCount(global_vec);
        ASSERT_EQ(globalMin, reference_min);
    }
}

TEST(Parallel_Operations_MPI, Test_uneven_count_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 99;
    if (rank == 0) {
        global_vec = getRandVec(count_size_vector);
    }
    int globalMin = parallelCount(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_min = SequentialCount(global_vec);
        ASSERT_EQ(globalMin, reference_min);
    }
}

TEST(Parallel_Operations_MPI, Test_uneven_count_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 77;
    if (rank == 0) {
        global_vec = getRandVec(count_size_vector);
    }
    int globalMin = parallelCount(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_min = SequentialCount(global_vec);
        ASSERT_EQ(globalMin, reference_min);
    }
}

TEST(Parallel_Operations_MPI, Test_even_count_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 86;
    if (rank == 0) {
        global_vec = getRandVec(count_size_vector);
    }
    int globalMin = parallelCount(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_min = SequentialCount(global_vec);
        ASSERT_EQ(globalMin, reference_min);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
     int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
