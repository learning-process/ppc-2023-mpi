// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include "task_2/skotin_a_grid_tor/grid_tor.h"

TEST(GridTorusTopology, GetNextNodeTest) {
    int rows = 3, cols = 3;

    int nextNode1 = GridTorusTopology::getNextNode(0, 8, rows, cols);
    EXPECT_EQ(nextNode1, 1);

    int nextNode2 = GridTorusTopology::getNextNode(6, 3, rows, cols);
    EXPECT_EQ(nextNode2, 7);

    int nextNode3 = GridTorusTopology::getNextNode(2, 6, rows, cols);
    EXPECT_EQ(nextNode3, 5);
}

TEST(GridTorusTopology, DataTransferFirstToLastTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto gridDims = GridTorusTopology::getGridDimensions(size);
    int rows = gridDims.first;
    int cols = gridDims.second;

    std::vector<int> dataToSend = {1, 2, 3};
    std::vector<int> receivedData(3);

    if (rank == 0) {
        GridTorusTopology::sendData(0, size - 1, 0, MPI_COMM_WORLD, dataToSend);
    } else if (rank == size - 1) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, 0, 0,
                                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        EXPECT_EQ(receivedData, dataToSend);
    }
}

TEST(GridTorusTopology, DataTransferLastToFirstTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto gridDims = GridTorusTopology::getGridDimensions(size);
    int rows = gridDims.first;
    int cols = gridDims.second;

    std::vector<int> dataToSend = {4, 5, 6};
    std::vector<int> receivedData(3);

    if (rank == size - 1) {
        GridTorusTopology::sendData(size - 1, 0, 0, MPI_COMM_WORLD, dataToSend);
    } else if (rank == 0) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, size - 1, 0,
                                            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        EXPECT_EQ(receivedData, dataToSend);
    }
}

TEST(GridTorusTopology, DataTransferFirstToNeighbourTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto gridDims = GridTorusTopology::getGridDimensions(size);
    int rows = gridDims.first;
    int cols = gridDims.second;

    std::vector<int> dataToSend = {7, 8, 9};
    std::vector<int> receivedData(3);

    if (rank == 0) {
        GridTorusTopology::sendData(0, 1, 0, MPI_COMM_WORLD, dataToSend);
    } else if (rank == 1) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, 0, 0,
                                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        EXPECT_EQ(receivedData, dataToSend);
    }
}

TEST(GridTorusTopology, DataTransferNeighbourToFirstTest) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto gridDims = GridTorusTopology::getGridDimensions(size);
    int rows = gridDims.first;
    int cols = gridDims.second;

    std::vector<int> dataToSend = {10, 11, 12};
    std::vector<int> receivedData(3);

    if (rank == 1) {
        GridTorusTopology::sendData(1, 0, 0, MPI_COMM_WORLD, dataToSend);
    } else if (rank == 0) {
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, 1, 0,
                                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        EXPECT_EQ(receivedData, dataToSend);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
