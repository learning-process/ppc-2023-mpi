// Copyright 2023 Kozlov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./shell_sort.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(ShellSortTest, SequentialVsMPI1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> par_arr{};
    std::vector<int> seq_arr{};

    parallel_sort(par_arr);

    if (rank == 0) {
        seq_sort(seq_arr);
        ASSERT_EQ(par_arr, seq_arr);
    }
}

TEST(ShellSortTest, SequentialVsMPI2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> par_arr{100, 102, 103, 104, 105};
    std::vector<int> seq_arr{100, 102, 103, 104, 105};

    parallel_sort(par_arr);

    if (rank == 0) {
        seq_sort(seq_arr);
        ASSERT_EQ(par_arr, seq_arr);
    }
}

TEST(ShellSortTest, SequentialVsMPI3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> par_arr{500, 400, 300, 200, 100};
    std::vector<int> seq_arr{500, 400, 300, 200, 100};

    parallel_sort(par_arr);

    if (rank == 0) {
        seq_sort(seq_arr);
        ASSERT_EQ(par_arr, seq_arr);
    }
}

TEST(ShellSortTest, SequentialVsMPI4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> arr(10);
    arr = {0, 55, 66, 12, -2, 33, 54, 234, 765, 654};

    std::vector<int> par_arr = arr;
    std::vector<int> seq_arr = arr;

    parallel_sort(par_arr);

    if (rank == 0) {
        seq_sort(seq_arr);
        ASSERT_EQ(par_arr, seq_arr);
    }
}

TEST(ShellSortTest, SequentialVsMPI5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> par_arr{11, 11, 12, 12, 13, 13, 14, 14, 15, 15};
    std::vector<int> seq_arr{11, 11, 12, 12, 13, 13, 14, 14, 15, 15};

    parallel_sort(par_arr);

    if (rank == 0) {
        seq_sort(seq_arr);
        ASSERT_EQ(par_arr, seq_arr);
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
