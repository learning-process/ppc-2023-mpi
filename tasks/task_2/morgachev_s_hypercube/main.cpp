// Copyright 2023 Morgachev Stepan
#include <gtest/gtest.h>
#include "./hypercube.h"

TEST(Hypercube, Test_Calculate_Next_Vertex) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = calculateNextVertex(0, 5);

    ASSERT_EQ(next, 1);
}

TEST(Hypercube, Test_Send_0_1) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 2) {
        int data = 0;
        if (rank == 0) {
            data = 100;
        }

        HypercubeSend(&data, 1, MPI_INT, 0, 1, 0, MPI_COMM_WORLD);

        if (rank == 1) {
            ASSERT_EQ(data, 100);
        }
    }
}

TEST(Hypercube, Test_Send_0_3) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        int data = 0;
        if (rank == 0) {
            data = 100;
        }

        HypercubeSend(&data, 1, MPI_INT, 0, 3, 0, MPI_COMM_WORLD);

        if (rank == 3) {
            ASSERT_EQ(data, 100);
        }
    }
}

TEST(hypercube_mpi, Test_Send_1_0) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 2) {
        int data = 0;
        if (rank == 1) {
            data = 100;
        }

        HypercubeSend(&data, 1, MPI_INT, 1, 0, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            ASSERT_EQ(data, 100);
        }
    }
}

TEST(hypercube_mpi, Test_Send_Array_1_6) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 8) {
        int data = 100;
        if (rank == 1) {
            data = 100;
        }

        HypercubeSend(&data, 1, MPI_INT, 1, 6, 0, MPI_COMM_WORLD);

        if (rank == 6) {
            ASSERT_EQ(data, 100);
        }
    }
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
