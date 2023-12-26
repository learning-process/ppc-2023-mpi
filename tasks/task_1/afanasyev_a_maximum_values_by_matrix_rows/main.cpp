// Copyright 2023 Afanasyev Aleksey
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <mpi.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/timer.hpp>
#include "./max_values_by_matrix_rows.h"

TEST(Max_Values_By_Matrix_Rows_MPI, one_element) {
	std::vector<int> matrix = {
		1
	};
	size_t rowSize = 1;
	size_t rowCount = 1;
	std::vector<int> maxValuesForTest = { 1 };
	std::vector<int> maxValuesParallel = CalculateMaxValuesByMatrixRowsParallel(matrix, rowSize, rowCount);

	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	if (worldRank == 0) {
		std::vector<int> maxValuesSequential = CalculateMaxValuesByMatrixRowsSequential(matrix, rowSize, rowCount);
		ASSERT_EQ(maxValuesForTest, maxValuesParallel);
		ASSERT_EQ(maxValuesParallel, maxValuesSequential);
	}
}

TEST(Max_Values_By_Matrix_Rows_MPI, one_row) {
	std::vector<int> matrix = {
		1, 2, 3, 2, 1
	};
	size_t rowSize = 5;
	size_t rowCount = 1;
	std::vector<int> maxValuesForTest = { 3 };
	std::vector<int> maxValuesParallel = CalculateMaxValuesByMatrixRowsParallel(matrix, rowSize, rowCount);

	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	if (worldRank == 0) {
		std::vector<int> maxValuesSequential = CalculateMaxValuesByMatrixRowsSequential(matrix, rowSize, rowCount);
		ASSERT_EQ(maxValuesForTest, maxValuesParallel);
		ASSERT_EQ(maxValuesParallel, maxValuesSequential);
	}
}

TEST(Max_Values_By_Matrix_Rows_MPI, standart_matrix) {
	std::vector<int> matrix = {
		1, 2, 3, 2, 1,
		1, 2, 4, 2, 1,
		1, 2, 5, 2, 1,
		1, 2, 6, 2, 1,
		1, 2, 7, 2, 1,
	};
	size_t rowSize = 5;
	size_t rowCount = 5;
	std::vector<int> maxValuesForTest = { 3, 4, 5, 6, 7 };
	std::vector<int> maxValuesParallel = CalculateMaxValuesByMatrixRowsParallel(matrix, rowSize, rowCount);

	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	if (worldRank == 0) {
		std::vector<int> maxValuesSequential = CalculateMaxValuesByMatrixRowsSequential(matrix, rowSize, rowCount);
		ASSERT_EQ(maxValuesForTest, maxValuesParallel);
		ASSERT_EQ(maxValuesParallel, maxValuesSequential);
	}
}

TEST(Max_Values_By_Matrix_Rows_MPI, matrix_without_max_elements) {
	std::vector<int> matrix = {
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
	};
	size_t rowSize = 5;
	size_t rowCount = 5;
	std::vector<int> maxValuesForTest = { 1, 1, 1, 1, 1 };
	std::vector<int> maxValuesParallel = CalculateMaxValuesByMatrixRowsParallel(matrix, rowSize, rowCount);

	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	if (worldRank == 0) {
		std::vector<int> maxValuesSequential = CalculateMaxValuesByMatrixRowsSequential(matrix, rowSize, rowCount);
		ASSERT_EQ(maxValuesForTest, maxValuesParallel);
		ASSERT_EQ(maxValuesParallel, maxValuesSequential);
	}
}

TEST(Max_Values_By_Matrix_Rows_MPI, matrix_with_10000_elements) {
	std::vector<int> matrix(10000);
	size_t rowSize = 100;
	size_t rowCount = 100;
	std::vector<int> maxValuesParallel = CalculateMaxValuesByMatrixRowsParallel(matrix, rowSize, rowCount);

	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	if (worldRank == 0) {
		std::vector<int> maxValuesSequential = CalculateMaxValuesByMatrixRowsSequential(matrix, rowSize, rowCount);
		ASSERT_EQ(maxValuesSequential, maxValuesParallel);
	}
}

int main(int argc, char** argv) {
	boost::mpi::environment env(argc, argv);
	boost::mpi::communicator world;
	::testing::InitGoogleTest(&argc, argv);
	::testing::TestEventListeners& listeners =
		::testing::UnitTest::GetInstance()->listeners();
	if (world.rank() != 0) {
		delete listeners.Release(listeners.default_result_printer());
	}
	return RUN_ALL_TESTS();
}
