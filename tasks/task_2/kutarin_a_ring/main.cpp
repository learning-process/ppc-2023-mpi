// Copyright 2023 Kutarin Aleksandr
#include <gtest/gtest.h>
#include <mpi.h>
#include <random>
#include <vector>
#include "task_2/kutarin_a_ring/ring.h"


TEST(ringTest, CheckRankAndSize) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ASSERT_TRUE(rank >= 0 && rank < size);
}


TEST(ringTest, CheckSendMessageInClockwiseDirection) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int source = 2;
    int receiver = 2;
    int message = 20;

    if (rank == source) {
        sendMessage(&message, source, receiver);
    } else if (rank == receiver) {
        ASSERT_EQ(message, 20);
    }
}


TEST(ringTest, Random) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int source = std::rand() % size;
    int receiver = std::rand() % size;
    int message = 0;
    if (size > 1) {
        if (rank == source)
             message = 20;
        sendMessage(&message, source, receiver);
        if (rank == receiver)
            ASSERT_EQ(message, 20);
    }
}


TEST(ringTest, CorrectWork) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int source = 1;
    int receiver = 1;
    int message = 0;
    if (size > 1) {
        if (rank == source) {
        message = 20;
        sendMessage(&message, source, receiver);
        }
        if (rank == receiver) {
            ASSERT_EQ(message, 20);
        }
    }
}



TEST(ringTest, EdgeCase) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int message = 42;
    sendMessage(&message, 0, 0);
    ASSERT_EQ(message, 42);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
     ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rcode = RUN_ALL_TESTS();
    MPI_Finalize();
    return rcode;
}
