// Copyright 2023 Trubin Artem
#include <gtest/gtest.h>
#include <mpi.h>
#include "./sleeping_barber.h"

TEST(Sleeping_barber, Test_Serve) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int number_of_chairs = 5;
    int need_to_serve = 25;
    int not_enough_chair = 0;
    int served = -1;
    if (size < 3) {
        if (rank == 0)
            served = sleeping_barber_sequential(number_of_chairs, need_to_serve, &not_enough_chair);
    } else {
        served = sleeping_barber(number_of_chairs, need_to_serve, &not_enough_chair);
        int flag;  // clearing buffer
        int mes;
        MPI_Status status;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        while (flag) {
            MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == 0) {
        ASSERT_EQ(served, 25);
    }
}

TEST(Sleeping_barber, Test_No_Chairs) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int number_of_chairs = 0;
    int need_to_serve = 25;
    int not_enough_chair = 0;
    int served = -1;
    if (size < 3) {
        if (rank == 0)
            served = sleeping_barber_sequential(number_of_chairs, need_to_serve, &not_enough_chair);
    } else {
        served = sleeping_barber(number_of_chairs, need_to_serve, &not_enough_chair);
        int flag;  // clearing buffer
        int mes;
        MPI_Status status;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        while (flag) {
            MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == 0) {
        ASSERT_EQ(served, 25);
    }
}

TEST(Sleeping_barber, Test_Sequential_Serve) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int number_of_chairs = 10;
    int need_to_serve = 30;
    int not_enough_chair = 0;
    int serve = -1;
    if (rank == 0) {
        serve = sleeping_barber_sequential(number_of_chairs, need_to_serve, &not_enough_chair);
        ASSERT_EQ(serve, 30);
    }
}

TEST(Sleeping_barber, Test_Nobody_To_Serve) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int number_of_chairs = 5;
    int need_to_serve = 0;
    int not_enough_chair = 0;
    int served = -1;
    if (size < 3) {
        if (rank == 0)
            served = sleeping_barber_sequential(number_of_chairs, need_to_serve, &not_enough_chair);
    } else {
        served = sleeping_barber(number_of_chairs, need_to_serve, &not_enough_chair);
        int flag;  // clearing buffer
        int mes;
        MPI_Status status;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        while (flag) {
            MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == 0) {
        ASSERT_EQ(served, 0);
    }
}

TEST(Sleeping_barber, Test_Negative_Value) {
    int not_enough_chair = 0;
    ASSERT_ANY_THROW(sleeping_barber(5, -10, &not_enough_chair));
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    MPI_Init(&argc, &argv);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
