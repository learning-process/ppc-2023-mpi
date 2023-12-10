// Copyright 2023 Kosarev Egor
#include <gtest/gtest.h>
#include "./quick_sort_merge.h"
#include <mpi.h>

TEST(quick_sort_merge, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::random_device dev;
    std::mt19937 gen(dev());
    int quantity = 50;
    int* a = new int[quantity];
    for (int i = 0; i < quantity; ++i) a[i] = gen();

    parallelQuickSort(a, quantity);

    if (rank == 0) {
        EXPECT_EQ(checking(a, quantity), true);
    }
    delete[] a;
}

TEST(quick_sort_merge, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::random_device dev;
    std::mt19937 gen(dev());
    int quantity = 3000;
    int* a = new int[quantity];
    for (int i = 0; i < quantity; ++i) a[i] = gen();

    parallelQuickSort(a, quantity);

    if (rank == 0) {
        EXPECT_EQ(checking(a, quantity), true);
    }
    delete[] a;
}

TEST(quick_sort_merge, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int quantity = 5000;
    int* a = new int[quantity];
    for (int i = 0; i < quantity; ++i) a[i] = i;

    parallelQuickSort(a, quantity);

    if (rank == 0) {
        bool flag = true;
        for (int i = 0; i < quantity; ++i) {
            if (a[i] != i) flag = false;
        }
        EXPECT_EQ(flag, true);
    }
    delete[] a;
}

TEST(quick_sort_merge, Test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::random_device dev;
    std::mt19937 gen(dev());
    int quantity = 10000;
    int* a = new int[quantity];
    for (int i = 0; i < quantity; ++i) a[i] = gen();

    parallelQuickSort(a, quantity);

    if (rank == 0) {
        EXPECT_EQ(checking(a, quantity), true);
    }
    delete[] a;
}

TEST(quick_sort_merge, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::random_device dev;
    std::mt19937 gen(dev());
    int quantity = 15000;
    int* a = new int[quantity];
    for (int i = 0; i < quantity; ++i) a[i] = gen();

    parallelQuickSort(a, quantity);

    if (rank == 0) {
        EXPECT_EQ(checking(a, quantity), true);
    }
    delete[] a;
}

int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
