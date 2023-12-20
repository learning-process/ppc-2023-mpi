// Copyright 2023 Savchuk Anton
#include<gtest/gtest.h>
#include "./sleeping_barber.h"

TEST(Slepping_Barber, Test_1_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_2_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_4_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_6_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_10_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::
    GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();

    MPI_Finalize();

    return result;
}

