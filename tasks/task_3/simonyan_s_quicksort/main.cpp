// Copyright 2023 Simonyan Suren
#include <gtest/gtest.h>
#include <algorithm>
#include "task_3/simonyan_s_quicksort/quicksort.h"

TEST(Allreduce, test_MPI_Double) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double arr1[] = { 1, 5, 7, 4, 2, 6, 8, 3, 5, 4, 11, 2, 4 };
    double arr2[] = { 1, 5, 7, 4, 2, 6, 8, 3, 5, 4, 11, 2, 4 };
    Sort(arr1, std::size(arr1));
    std::sort(std::begin(arr2), std::end(arr2));
    int ch = 0;
    for (int i = 0; i < std::size(arr1); i++)
        if (arr1[i] == arr2[i])
            ch++;
    ASSERT_EQ(ch, std::size(arr1));
}

int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
