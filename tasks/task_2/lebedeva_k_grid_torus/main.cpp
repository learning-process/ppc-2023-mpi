// Copyright 2023 Lebedeva Ksenia
#include <gtest/gtest.h>
#include <vector>
#include "./grid_torus.h"

TEST(Torus_Topology, Test_Tor1) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender = 0, receiver = 0;
    int data = 0;

    if (rank == sender) {
        data = 100;
    }

    sendData(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

    if (rank == receiver) {
        ASSERT_EQ(data, 100);
    }
}

TEST(Torus_Topology, Test_Tor2) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender = size / 2, receiver = 0;
    int data = 0;

    if (rank == sender) {
        data = 100;
    }

    sendData(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

    if (rank == receiver) {
        ASSERT_EQ(data, 100);
    }
}

TEST(Torus_Topology, Test_Tor3) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender = 0, receiver = size - 1;
    int data = 0;

    if (rank == sender) {
        data = 100;
    }

    sendData(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

    if (rank == receiver) {
        ASSERT_EQ(data, 100);
    }
}

TEST(Torus_Topology, Test_Tor4) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 8) {
        int sender = 1, receiver = 6;
        int data = 100;
        if (rank == sender) {
            data = 100;
        }

        sendData(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

        if (rank == receiver) {
            ASSERT_EQ(data, 100);
        }
    }
}

TEST(Torus_Topology, Test_Tor5) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int sender = size - 1, receiver = 0;
    int data = 100;

    if (rank == sender) {
        data = 100;
    }

    sendData(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

    if (rank == receiver) {
        ASSERT_EQ(data, 100);
    }
}


int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
