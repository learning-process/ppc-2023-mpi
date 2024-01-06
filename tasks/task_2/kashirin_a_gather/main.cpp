// Copyright 2023 Kashirin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./gather.h"

TEST(TEST_GATHER, Test_1) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> sendbuf = { 1 };
    std::vector<int> recvbuf(numProc);
    Gather(sendbuf.data(), 1, MPI_INT, recvbuf.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < recvbuf.size(); i++)
            EXPECT_EQ(1, recvbuf[i]);
    }
}

TEST(TEST_GATHER, Test_int) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> sendbuf(4);
    for (int i = 0; i < sendbuf.size(); i++) sendbuf[i] = rank * 4 + i;
    std::vector<int> recvbuf(numProc * sendbuf.size());

    Gather(sendbuf.data(), sendbuf.size(), MPI_INT,
        recvbuf.data(), sendbuf.size(), MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < recvbuf.size(); i++)
            EXPECT_EQ(i, recvbuf[i]);
    }
}

TEST(TEST_GATHER, Test_float) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<float> sendbuf(4);
    for (int i = 0; i < sendbuf.size(); i++) sendbuf[i] = rank * 4 + i + 0.5;
    std::vector<float> recvbuf(numProc * sendbuf.size());

    Gather(sendbuf.data(), sendbuf.size(), MPI_FLOAT,
        recvbuf.data(), sendbuf.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < recvbuf.size(); i++)
            EXPECT_EQ(i + 0.5, recvbuf[i]);
    }
}

TEST(TEST_GATHER, Test_double) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<double> sendbuf(4);
    for (int i = 0; i < sendbuf.size(); i++) sendbuf[i] =  rank * 4 + i + 0.5;
    std::vector<double> recvbuf(numProc * sendbuf.size());

    Gather(sendbuf.data(), sendbuf.size(), MPI_DOUBLE,
        recvbuf.data(), sendbuf.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < recvbuf.size(); i++)
            EXPECT_EQ(i + 0.5, recvbuf[i]);
    }
}

TEST(TEST_GATHER, Test_time) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<float> sendbuf(4);
    for (int i = 0; i < sendbuf.size(); i++) sendbuf[i] =  rank * 4 + i + 0.5;
    std::vector<float> recvbuf(numProc * sendbuf.size());
    std::vector<float> recvbuf2(numProc * sendbuf.size());
    double time1, time2, time3;
    if (rank == 0)time1 = MPI_Wtime();
    Gather(sendbuf.data(), sendbuf.size(), MPI_FLOAT,
        recvbuf.data(), sendbuf.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0)time2 = MPI_Wtime();
    MPI_Gather(sendbuf.data(), sendbuf.size(), MPI_FLOAT,
        recvbuf2.data(), sendbuf.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        time3 = MPI_Wtime();
        std::cout << "Gather time = " << time2 - time1
            << std::endl << "MPI_Gather time = " << time3 - time2 << std::endl;
        for (int i = 0; i < recvbuf.size(); i++) {
            EXPECT_EQ(i + 0.5, recvbuf[i]);
            EXPECT_EQ(i + 0.5, recvbuf2[i]);
        }
    }
}

TEST(TEST_GATHER, Test_wrong_root) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::vector<int> sendbuf(4);
    for (int i = 0; i < sendbuf.size(); i++) sendbuf[i] = rank * 4 + i;
    std::vector<int> recvbuf(numProc * sendbuf.size());
    if (rank == 0) {
        EXPECT_ANY_THROW(Gather(sendbuf.data(), sendbuf.size(), MPI_INT,
            recvbuf.data(), sendbuf.size(), MPI_INT, -1, MPI_COMM_WORLD));
    }
}

TEST(TEST_GATHER, Test_wrong_type) {
    int numProc;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> sendbuf(4);
    for (int i = 0; i < sendbuf.size(); i++) sendbuf[i] = rank * 4 + i;
    std::vector<int> recvbuf(numProc * sendbuf.size());
    if (rank == 0) {
        EXPECT_ANY_THROW(Gather(sendbuf.data(), sendbuf.size(), MPI_CHAR,
            recvbuf.data(), sendbuf.size(), MPI_CHAR, 0, MPI_COMM_WORLD));
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return result_code;
}
