// Copyright 2023 Karagodin Andrey

#include <gtest/gtest.h>
#include <mpi.h>
#include <task_3/karagodin_a_shell_betcher/shell_betcher.h>

TEST(Parallel_Operations_MPI, Test_Sorting_Algo_1) {
    int rank = 0;
    const int ArraySize = 8;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = GenerateArrayRandom(ArraySize);
    }
    std::vector<int> parallel_sort = ShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = ShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Parallel_Operations_MPI, Test_Sorting_Algo_2) {
    int rank = 0;
    const int ArraySize = 32;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = GenerateArrayRandom(ArraySize);
    }
    std::vector<int> parallel_sort = ShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = ShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Parallel_Operations_MPI, Test_Sorting_Algo_3) {
    int rank = 0;
    const int ArraySize = 64;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = GenerateArrayRandom(ArraySize);
    }
    std::vector<int> parallel_sort = ShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = ShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Parallel_Operations_MPI, Test_Sorting_Algo_4) {
    int rank = 0;
    const int ArraySize = 256;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = GenerateArrayRandom(ArraySize);
    }
    std::vector<int> parallel_sort = ShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = ShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Parallel_Operations_MPI, Test_Sorting_Algo_5) {
    int rank = 0;
    const int ArraySize = 512;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = GenerateArrayRandom(ArraySize);
    }
    std::vector<int> parallel_sort = ShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = ShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    int rank = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
