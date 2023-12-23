// Copyright 2023 Alexseev Danila

#include <gtest/gtest.h>
#include "./ring_topology.h"

TEST(ringSendMessage, randomProcessesPos) {
    int rank, size, sourceRank, receiverRank, message = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) {
            ASSERT_EQ(1, 1);
        }
    } else {
        sourceRank = getRandomInt(0, size - 1);
        receiverRank = getRandomInt(0, size - 1);
        if (rank == sourceRank) {
            message = 200;
        }
        sendMessage(&message, sourceRank, receiverRank);
        if (rank == receiverRank) {
            ASSERT_EQ(200, message);
        }
    }
}

TEST(ringSendMessage, firstToLast) {
    int rank, size, sourceRank, receiverRank, message = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) {
            ASSERT_EQ(1, 1);
        }
    } else {
        sourceRank = 0;
        receiverRank = size - 1;
        if (rank == sourceRank) {
            message = 200;
        }
        sendMessage(&message, sourceRank, receiverRank);
        if (rank == receiverRank) {
            ASSERT_EQ(200, message);
        }
    }
}

TEST(ringSendMessage, sameProcess) {
    int rank, size, sourceRank, receiverRank, message = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) {
            ASSERT_EQ(1, 1);
        }
    } else {
        int process = getRandomInt(0, size - 1);
        sourceRank = process;
        receiverRank = process;
        if (rank == sourceRank) {
            message = 200;
        }
        sendMessage(&message, sourceRank, receiverRank);
        if (rank == receiverRank) {
            ASSERT_EQ(200, message);
        }
    }
}

TEST(ringSendMessage, sourceLargerThanReceiver) {
    int rank, size, sourceRank, receiverRank, message = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) {
            ASSERT_EQ(1, 1);
        }
    } else {
        sourceRank = size - 2;
        receiverRank = size - 1;
        if (rank == sourceRank) {
            message = 200;
        }
        sendMessage(&message, sourceRank, receiverRank);
        if (rank == receiverRank) {
            ASSERT_EQ(200, message);
        }
    }
}

TEST(ringSendMessage, firstAndMiddle) {
    int rank, size, sourceRank, receiverRank, message = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) {
            ASSERT_EQ(1, 1);
        }
    } else {
        sourceRank = 0;
        receiverRank = size/2;
        if (rank == sourceRank) {
            message = 200;
        }
        sendMessage(&message, sourceRank, receiverRank);
        if (rank == receiverRank) {
            ASSERT_EQ(200, message);
        }
    }
}

int main(int argc, char** argv) {
    int res = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    res = RUN_ALL_TESTS();
    MPI_Finalize();
    return res;
}
