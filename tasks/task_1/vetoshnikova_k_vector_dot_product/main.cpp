#include "vector_dot_product.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Parallel_Vector_Dot_Product, Test_equals_0) {

	int ProcNum, ProcRank;
	int count_size_vector = 4;


	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	vector<int> a{ 1, 1, 1, 1 };
	vector<int> b{ 1, -1, 1, -1 };


	int res = getParallelOperations(a, b, count_size_vector);

	if (ProcRank == 0) {
		int refRes = getSequentialOperations(a, b, count_size_vector);
		ASSERT_EQ(res, refRes);
	}
}

TEST(Parallel_Vector_Dot_Product, Test_random_vectors) {

	int ProcNum, ProcRank;
	int count_size_vector = 22;


	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	vector<int> a = vector_generation(5, 40, count_size_vector);
	vector<int> b = vector_generation(2, 25, count_size_vector);


	int res = getParallelOperations(a, b, count_size_vector);

	if (ProcRank == 0) {
		int refRes = getSequentialOperations(a, b, count_size_vector);
		ASSERT_EQ(res, refRes);
	}
}

TEST(Parallel_Vector_Dot_Product, Test_negative_elements) {

	int ProcNum, ProcRank;
	int count_size_vector = 15;


	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	vector<int> a = vector_generation(-20, 0, count_size_vector);
	vector<int> b = vector_generation(-5, 50, count_size_vector);


	int res = getParallelOperations(a, b, count_size_vector);

	if (ProcRank == 0) {
		int refRes = getSequentialOperations(a, b, count_size_vector);
		ASSERT_EQ(res, refRes);
	}
}

TEST(Parallel_Vector_Dot_Product, Test_0_vector) {

	int ProcNum, ProcRank;
	int count_size_vector = 7;


	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	vector<int> a{0, 0, 0, 0, 0, 0, 0};
	vector<int> b = vector_generation(10, 100, count_size_vector);


	int res = getParallelOperations(a, b, count_size_vector);

	if (ProcRank == 0) {
		int refRes = getSequentialOperations(a, b, count_size_vector);
		ASSERT_EQ(res, refRes);
	}
}

TEST(Parallel_Vector_Dot_Product, Test_random_vectors_2) {

	int ProcNum, ProcRank;
	int count_size_vector = 28;


	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	vector<int> a = vector_generation(-50, 0, count_size_vector);
	vector<int> b = vector_generation(-100, 0, count_size_vector);


	int res = getParallelOperations(a, b, count_size_vector);

	if (ProcRank == 0) {
		int refRes = getSequentialOperations(a, b, count_size_vector);
		ASSERT_EQ(res, refRes);
	}
}

int main(int argc, char** argv)
{
	int result_code = 0;

	::testing::InitGoogleTest(&argc, argv);
	::testing::TestEventListeners& listeners =
		::testing::UnitTest::GetInstance()->listeners();

	if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
		MPI_Abort(MPI_COMM_WORLD, -1);

	result_code = RUN_ALL_TESTS();

	MPI_Finalize();

	

}