// Copyright 2023 Sredneva Anastasiya
#include <gtest/gtest.h>
#include "./hypercube.h"

TEST(TEST_MPI, Test_1) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int start = 0;
    int end = 3;
    int data = 0;
    if (ProcNum >= 4) {
        if (ProcRank == start) {
            data = 15;
        }
        send_data_hypercube(data, start, end);
        if (ProcRank == end) {
            ASSERT_EQ(data, 15);
        }
    }
}

TEST(TEST_MPI, Test_2) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int start = 2;
    int end = 5;
    int data = 0;
    if (ProcNum >= 8) {
        if (ProcRank == start) {
            data = 15;
        }
        send_data_hypercube(data, start, end);
        if (ProcRank == end) {
            ASSERT_EQ(data, 15);
        }
    }
}

TEST(TEST_MPI, Test_3) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int start = 8;
    int end = 7;
    int data = 0;
    if (ProcNum >= 16) {
        if (ProcRank == start) {
            data = 15;
        }
        send_data_hypercube(data, start, end);
        if (ProcRank == end) {
            ASSERT_EQ(data, 15);
        }
    }
}

TEST(TEST_MPI, Test_4) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int start = 4;
    int end = 5;
    int data = 0;
    if (ProcNum >= 8) {
        if (ProcRank == start) {
            data = 15;
        }
        send_data_hypercube(data, start, end);
        if (ProcRank == end) {
            ASSERT_EQ(data, 15);
        }
    }
}

TEST(TEST_MPI, Test_5) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int start = 7;
    int end = 1;
    int data = 0;
    if (ProcNum >= 8) {
        if (ProcRank == start) {
            data = 15;
        }
        send_data_hypercube(data, start, end);
        if (ProcRank == end) {
            ASSERT_EQ(data, 15);
        }
    }
}


int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    MPI_Init(&argc, &argv);
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
