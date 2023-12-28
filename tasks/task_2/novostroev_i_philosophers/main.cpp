// Copyright 2023 Novostroev Ivan
#include <gtest/gtest.h>
#include "task_2/novostroev_i_philosophers/philosophers.h"

TEST(Philosophers, Test1) {
    int rank = 0;
    int size = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Philosophers philosopher(rank, size);
    philosopher.initialize();

    ASSERT_EQ(philosopher.getPhilosopherID(), rank);
    ASSERT_EQ(philosopher.getTotalPhilosophers(), size);
    ASSERT_EQ(philosopher.getLeftNeighbor(), (rank + size - 1) % size);
    ASSERT_EQ(philosopher.getRightNeighbor(), (rank + 1) % size);
    ASSERT_EQ(philosopher.getState(), THINKING);
}

TEST(Philosophers, Test2) {
    int rank = 0;
    int size = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Philosophers philosopher(rank, size);
    philosopher.initialize();

    philosopher.think();
    ASSERT_EQ(philosopher.getState(), THINKING);
}

TEST(Philosophers, Test3) {
    int rank = 0;
    int size = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        Philosophers philosopher(rank, size);
        philosopher.initialize();

        philosopher.hungry();
        ASSERT_EQ(philosopher.getState(), EATING);
    } else {
        ASSERT_TRUE(true);
    }
}

TEST(Philosophers, Test4) {
    int rank = 0;
    int size = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        Philosophers philosopher(rank, size);
        philosopher.initialize();

        philosopher.think();
        philosopher.hungry();
        ASSERT_EQ(philosopher.getState(), EATING);
    } else {
        ASSERT_TRUE(true);
    }
}

TEST(Philosophers, Test5) {
    int rank = 0;
    int size = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        Philosophers philosopher(rank, size);
        philosopher.initialize();

        for (int i = 0; i < size; ++i) {
            philosopher.think();
            philosopher.hungry();
        }
    }

    ASSERT_TRUE(true);
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
     ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
