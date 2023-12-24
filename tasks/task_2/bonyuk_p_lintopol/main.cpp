  // Copyright 2023 Bonyuk Petr

#include <mpi.h>
#include <gtest/gtest.h>
#include "task_2/bonyuk_p_lintopol/lintopol.h"

const int TEST_DATA_VALUE = 100;

int runLinearTopologyTest(int sender, int receiver, MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    int data = 0;
    if (rank == sender) {
        data = TEST_DATA_VALUE;
    }

    Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, sender, receiver, 0, comm);

    if (rank == receiver) {
        return data;
    } else {
       return -1;
    }
}

void testWithMultipleProcesses(std::function<int(MPI_Comm)> testFunc, int expectedValue, MPI_Comm comm) {
    int world_rank;
    MPI_Comm_rank(comm, &world_rank);

    int value = testFunc(comm);

    if (world_rank == 0) {
        int result;
        MPI_Reduce(&value, &result, 1, MPI_INT, MPI_MAX, 0, comm);
        ASSERT_EQ(result, expectedValue);
    } else if (value != -1) {
        MPI_Reduce(&value, nullptr, 1, MPI_INT, MPI_MAX, 0, comm);
    }
    MPI_Barrier(comm);
}

TEST(LinearTopol, TestMiddleSendDec) {
    testWithMultipleProcesses([](MPI_Comm comm) {
        return runLinearTopologyTest(2, 0, comm);
    }, TEST_DATA_VALUE, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestFirstSend) {
    testWithMultipleProcesses([](MPI_Comm comm) {
        return runLinearTopologyTest(0, 3, comm);
    }, TEST_DATA_VALUE, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestLastSend) {
    testWithMultipleProcesses([](MPI_Comm comm) {
        return runLinearTopologyTest(3, 0, comm);
    }, TEST_DATA_VALUE, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestMultipleSends) {
    testWithMultipleProcesses([](MPI_Comm comm) {
        return runLinearTopologyTest(2, 3, comm);
    }, TEST_DATA_VALUE, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestAnotherMultipleSends) {
    testWithMultipleProcesses([](MPI_Comm comm) {
        return runLinearTopologyTest(0, 2, comm);
    }, TEST_DATA_VALUE, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int result_code = RUN_ALL_TESTS();
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    if (world_rank != 0) {
        exit(0);
    }
    return result_code;
}
