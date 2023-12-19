// Copyright 2023 Akopyan Zal
#include <gtest/gtest.h>
#include <mpi.h>
#include "./sleeping_hairdresser.h"

TEST(sleeping_hairdresser, two) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        ASSERT_NO_THROW(barber(rank, 2, size-2));
    } else if (rank == 1) {
        ASSERT_NO_THROW(line(2, size - 2));
    } else {
        ASSERT_NO_THROW(customer(rank));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, five) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        ASSERT_NO_THROW(barber(rank, 5, size-2));
    } else if (rank == 1) {
        ASSERT_NO_THROW(line(5, size-2));
    } else {
        ASSERT_NO_THROW(customer(rank));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, ten) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        ASSERT_NO_THROW(barber(rank, 10, size - 2));
    } else if (rank == 1) {
        ASSERT_NO_THROW(line(10, size - 2));
    } else {
        ASSERT_NO_THROW(customer(rank));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, fifteen) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        ASSERT_NO_THROW(barber(rank, 15, size - 2));
    } else if (rank == 1) {
        ASSERT_NO_THROW(line(15, size - 2));
    } else {
        ASSERT_NO_THROW(customer(rank));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, one) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        ASSERT_NO_THROW(barber(rank, 1, size - 2));
    } else if (rank == 1) {
        ASSERT_NO_THROW(line(1, size - 2));
    } else {
        ASSERT_NO_THROW(customer(rank));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
     int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();

    MPI_Finalize();

    return result;
}
