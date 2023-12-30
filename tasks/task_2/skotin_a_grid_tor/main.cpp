// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include "task_2/skotin_a_grid_tor/grid_tor.h"

TEST(GridTorusTopology, GetNextNodeTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int rows = 3, cols = 3;

        EXPECT_EQ(getNextNode(0, 1, rows, cols), 1);
        EXPECT_EQ(getNextNode(0, 3, rows, cols), 3);
        EXPECT_EQ(getNextNode(8, 0, rows, cols), 0);
    }
}

TEST(GridTorusTopology, DataTransferFirstToLastTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        return;
    }

    std::vector<int> dataToSend = { 1, 2, 3 };
    std::vector<int> receivedData(3);

    if (rank == 0) {
        sendData(0, size - 1, dataToSend, MPI_COMM_WORLD);
    } else if (rank == size - 1) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, 0, 0,
                                      MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ASSERT_EQ(receivedData, dataToSend);
    }
}

TEST(GridTorusTopology, DataTransferLastToFirstTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        return;
    }

    std::vector<int> dataToSend = { 4, 5, 6 };
    std::vector<int> receivedData(3);

    if (rank == size - 1) {
        sendData(size - 1, 0, dataToSend, MPI_COMM_WORLD);
    } else if (rank == 0) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, size - 1, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ASSERT_EQ(receivedData, dataToSend);
    }
}

TEST(GridTorusTopology, DataTransferFirstToNeighbourTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> dataToSend = { 7, 8, 9 };
    std::vector<int> receivedData(3);

    if (size < 2) {
        return;
    }

    if (rank == 0) {
        sendData(0, 1, dataToSend, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, 0, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ASSERT_EQ(receivedData, dataToSend);
    }
}

TEST(GridTorusTopology, DataTransferNeighbourToFirstTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> dataToSend = { 10, 11, 12 };
    std::vector<int> receivedData(3);

    if (size < 2) {
        return;
    }

    if (rank == 1) {
        sendData(1, 0, dataToSend, MPI_COMM_WORLD);
    } else if (rank == 0) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, 1, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ASSERT_EQ(receivedData, dataToSend);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
