// Copyright 2023 Fedotov Kirill

#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./gather.h"

using std::vector;

TEST(MPI_Gather_Custom, Same_Types) {
    // Arrange
    constexpr int ROOT = 0;
    constexpr int SENDSIZE = 500;
    int rank, size;
    vector<int> sendbuf(SENDSIZE);
    vector<int> recvbuf;
    vector<int> exp;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int k = rank * sendbuf.size();
    for (int& i : sendbuf) {
        i = ++k;
    }
    if (rank == 0) {
        recvbuf.resize(size * sendbuf.size());
        exp.resize(recvbuf.size());
    }

    // Act
    MPI_Gather_c(sendbuf.data(), sendbuf.size(), MPI_INT,
                 recvbuf.data(), sendbuf.size(), MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Gather(sendbuf.data(), sendbuf.size(), MPI_INT,
               exp.data(), sendbuf.size(), MPI_INT, ROOT, MPI_COMM_WORLD);

    // Assert
    if (rank == 0) {
        ASSERT_EQ(exp, recvbuf);
    }
}

TEST(MPI_Gather_Custom, Derived_Types) {
    // Arrange
    constexpr int ROOT = 0;
    constexpr int SENDSIZE = 1000;
    int rank, size;
    vector<int16_t> sendbuf(SENDSIZE);
    vector<int> recvbuf;
    vector<int> exp;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int k = rank * sendbuf.size();
    for (int16_t& i : sendbuf) {
        i = ++k;
    }
    if (rank == 0) {
        recvbuf.resize(size * sendbuf.size() / 2);
        exp.resize(recvbuf.size());
    }

    // Act
    MPI_Gather_c(sendbuf.data(), sendbuf.size(), MPI_SHORT,
                 recvbuf.data(), sendbuf.size() / 2, MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Gather(sendbuf.data(), sendbuf.size(), MPI_SHORT,
               exp.data(), sendbuf.size() / 2, MPI_INT, ROOT, MPI_COMM_WORLD);

    // Assert
    if (rank == 0) {
        ASSERT_EQ(exp, recvbuf);
    }
}

TEST(MPI_Gather_Custom, Recieve_Comp_Type) {
    // Arrange
    constexpr int ROOT = 0;
    constexpr int SENDSIZE = 500;
    int rank, size;
    MPI_Datatype comptype;
    vector<int> sendbuf(SENDSIZE);
    vector<int> recvbuf;
    vector<int> exp;

    MPI_Type_contiguous(sendbuf.size(), MPI_INT, &comptype);
    MPI_Type_commit(&comptype);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int k = rank * sendbuf.size();
    for (int& i : sendbuf) {
        i = ++k;
    }
    if (rank == 0) {
        recvbuf.resize(size * sendbuf.size());
        exp.resize(recvbuf.size());
    }

    // Act
    MPI_Gather_c(sendbuf.data(), sendbuf.size(), MPI_INT, recvbuf.data(), 1, comptype, ROOT, MPI_COMM_WORLD);
    MPI_Gather(sendbuf.data(), sendbuf.size(), MPI_INT, exp.data(), 1, comptype, ROOT, MPI_COMM_WORLD);

    // Assert
    if (rank == 0) {
        ASSERT_EQ(exp, recvbuf);
    }
}

TEST(MPI_Gather_Custom, Same_Comp_Types) {
    // Arrange
    constexpr int ROOT = 0;
    constexpr int SENDSIZE = 500;
    int rank, size;
    MPI_Datatype comptype;
    vector<int> sendbuf(SENDSIZE);
    vector<int> recvbuf;
    vector<int> exp;

    MPI_Type_contiguous(sendbuf.size(), MPI_INT, &comptype);
    MPI_Type_commit(&comptype);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int k = rank * sendbuf.size();
    for (int& i : sendbuf) {
        i = ++k;
    }
    if (rank == 0) {
        recvbuf.resize(size * sendbuf.size());
        exp.resize(recvbuf.size());
    }

    // Act
    MPI_Gather_c(sendbuf.data(), 1, comptype, recvbuf.data(), 1, comptype, ROOT, MPI_COMM_WORLD);
    MPI_Gather(sendbuf.data(), 1, comptype, exp.data(), 1, comptype, ROOT, MPI_COMM_WORLD);

    // Assert
    if (rank == 0) {
        ASSERT_EQ(exp, recvbuf);
    }
}

TEST(MPI_Gather_Custom, Send_Comp_Type) {
    // Arrange
    constexpr int ROOT = 0;
    constexpr int SENDSIZE = 500;
    int rank, size;
    MPI_Datatype comptype;
    vector<int> sendbuf(SENDSIZE);
    vector<int> recvbuf;
    vector<int> exp;

    MPI_Type_contiguous(sendbuf.size(), MPI_INT, &comptype);
    MPI_Type_commit(&comptype);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int k = rank * sendbuf.size();
    for (int& i : sendbuf) {
        i = ++k;
    }
    if (rank == 0) {
        recvbuf.resize(size * sendbuf.size());
        exp.resize(recvbuf.size());
    }

    // Act
    MPI_Gather_c(sendbuf.data(), 1, comptype, recvbuf.data(), sendbuf.size(), MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Gather(sendbuf.data(), 1, comptype, exp.data(), sendbuf.size(), MPI_INT, ROOT, MPI_COMM_WORLD);

    // Assert
    if (rank == 0) {
        ASSERT_EQ(exp, recvbuf);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
