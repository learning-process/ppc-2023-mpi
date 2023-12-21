// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include "task_2/skotin_a_grid_tor/grid_alg.h"

TEST(TorusGridTest, SendToAdjacentNode) {
    TorusGrid grid;
    int testData = 0;
    if (grid.rank == 0) {
        testData = 10;
        grid.sendToNeighbor(&testData, 1, MPI_INT, 0, 1, 0);
    } else if (grid.rank == 1) {
        MPI_Status status;
        MPI_Recv(&testData, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        ASSERT_EQ(testData, 10);
    }
}

TEST(TorusGridTest, SendToDistantNode) {
    TorusGrid grid;
    int testData = 0;

    if (grid.rank == 0) {
        testData = 20;
        grid.sendToNeighbor(&testData, 1, MPI_INT, 0, 2, 0);
    } else if (grid.rank == 2) {
        MPI_Status status;
        MPI_Recv(&testData, 1, MPI_INT, MPI_ANY_SOURCE, 0,
                                  MPI_COMM_WORLD, &status);
        ASSERT_EQ(testData, 20);
    }
}

TEST(TorusGridTest, SendToNodeInReverse) {
    TorusGrid grid;
    int testData = 0;

    if (grid.rank == 3) {
        testData = 30;
        grid.sendToNeighbor(&testData, 1, MPI_INT, 3, 0, 0);
    } else if (grid.rank == 0) {
        MPI_Status status;
        MPI_Recv(&testData, 1, MPI_INT, MPI_ANY_SOURCE, 0,
                                  MPI_COMM_WORLD, &status);
        ASSERT_EQ(testData, 30);
    }
}

TEST(TorusGridTest, SendToSelf) {
    TorusGrid grid;
    int testData = 40;

    grid.sendToNeighbor(&testData, 1, MPI_INT, grid.rank, grid.rank, 0);
    ASSERT_EQ(testData, 40);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int result = RUN_ALL_TESTS();

    MPI_Finalize();
    return result;
}
