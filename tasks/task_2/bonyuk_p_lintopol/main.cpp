﻿  // Copyright 2023 Bonyuk Petr

#include <mpi.h>
#include <gtest/gtest.h>
#include "task_2/bonyuk_p_lintopol/lintopol.h"

TEST(LinearTopol, test_one) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int send = 0;
    const int rec = size - 1;
    int data = 0;

    if (rank == send) data = 100;

    Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, send, rec, 0, MPI_COMM_WORLD);

    if (rank == rec) {
        ASSERT_EQ(100, data);
    }
}
TEST(LinearTopol, test_two) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int send = size - 1;
    const int rec = 0;
    int data = 0;

    if (rank == send) data = 100;

    Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, send, rec, 0, MPI_COMM_WORLD);

    if (rank == rec) {
        ASSERT_EQ(100, data);
    }
}

TEST(LinearTopol, test_three) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int send = 0;
    const int rec = 1;
    int data = 0;

    if (rank == send) data = 100;

    Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, send, rec, 0, MPI_COMM_WORLD);

    if (rank == rec) {
        ASSERT_EQ(100, data);
    }
}

TEST(LinearTopol, test_four) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int send = 1;
    const int rec = 0;
    int data = 0;

    if (rank == send) data = 100;

    Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, send, rec, 0, MPI_COMM_WORLD);

    if (rank == rec) {
        ASSERT_EQ(100, data);
    }
}

TEST(LinearTopol, test_five) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int send = 1;
    const int rec = size - 1;
    int data = 0;

    if (rank == send) data = 100;

    Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, send, rec, 0, MPI_COMM_WORLD);

    if (rank == rec) {
        ASSERT_EQ(100, data);
    }
}
int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
