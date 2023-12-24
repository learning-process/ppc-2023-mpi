  // Copyright 2023 Bonyuk Petr

#include <mpi.h>
#include <gtest/gtest.h>
#include "task_2/bonyuk_p_lintopol/lintopol.h"

TEST(LinearTopol, test_one) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = (rank == 0) ? 100 : 0;

    if (size > 1) {
        Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, 0, size - 1, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == size - 1) {
       ASSERT_EQ(data, 100);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(LinearTopol, test_two) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = (rank == 0) ? 100 : 0;

    if (size > 1) {
        Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, size - 1, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == size - 1) {
        ASSERT_EQ(data, 100);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(LinearTopol, test_three) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = (rank == 0) ? 100 : 0;

    if (size > 1) {
        Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, 0, 1, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == size - 1) {
        ASSERT_EQ(data, 100);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(LinearTopol, test_four) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = (rank == 0) ? 100 : 0;

    if (size > 1) {
        Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, 1, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == size - 1) {
        ASSERT_EQ(data, 100);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(LinearTopol, test_five) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = (rank == 0) ? 100 : 0;

    if (size > 1) {
         Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, 1, size - 1, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == size - 1) {
        ASSERT_EQ(data, 100);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
