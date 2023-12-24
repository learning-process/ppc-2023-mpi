// Copyright 2023 Bonyuk Petr
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "task_1/bonyuk_p_lintopol/lintopol.h"

const int TEST_DATA_VALUE = 500;

void initializeTestData(int rank, int sender, int& data) {
	if (rank == sender) {
		data = TEST_DATA_VALUE;
	}
}

void runLinearTopologyTest(int sender, int receiver, MPI_Comm comm) {
	int rank, size;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);

	int data = 0;
	initializeTestData(rank, sender, data);

	Sends_data_lin_acr_the_topol(&data, 1, MPI_INT, sender, receiver, 0, comm);

	if (rank == receiver) {
		ASSERT_EQ(TEST_DATA_VALUE, data);
	}
}

TEST(LinearTopol, TestMiddleSendDec) {
	runLinearTopologyTest(2, 0, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestFirstSend) {
	runLinearTopologyTest(0, 3, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestLastSend) {
	runLinearTopologyTest(3, 0, MPI_COMM_WORLD);
}

TEST(LinearTopol, TestMultipleSends) {
	runLinearTopologyTest(2, 3, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
	int result_code = 0;

	::testing::InitGoogleTest(&argc, argv);
	::testing::TestEventListeners& listeners =
		::testing::UnitTest::GetInstance()->listeners();

	if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
		std::cerr << "MPI initialization failed" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

	result_code = RUN_ALL_TESTS();

	MPI_Finalize();

	return result_code;
}