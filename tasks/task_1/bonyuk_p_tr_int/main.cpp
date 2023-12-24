#include "gtest/gtest.h"
#include <iostream>
#include <mpi.h>
#include "task_1/bonyuk_p_tr_int/tr_int.h"

#define ERRORRATE 0.1

TEST(TrapecIntegral, Fconst) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int N = 100;
	const double a = 0, b = 10;
	const double real_var = 10;

	double global_sum = TrapecIntegr(a, b, const_function, N);

	if (rank == 0) {
		ASSERT_LT(std::abs(real_var - global_sum), ERRORRATE);
	}
}

TEST(TrapecIntegral, standardfun) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int N = 100;
	const double a = -1;
	const double b = 1;
	const double real_var = 1.5707963267948966;


	double global_sum = TrapecIntegr(a, b, standard_function, N);

	if (rank == 0) {
		ASSERT_LT(std::abs(real_var - global_sum), ERRORRATE);
	}
}

TEST(TrapecIntegral, complfun) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int N = 100;
	const double a = -1;
	const double b = 1;
	const double real_var = 0.23042249506047835;

	double global_sum = TrapecIntegr(a, b, complex_function, N);

	if (rank == 0) {
		ASSERT_LT(std::abs(real_var - global_sum), ERRORRATE);
	}
}

TEST(TrapecIntegral, sqrtfun) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int N = 100;
	const double a = -1;
	const double b = 1;
	const double real_var = 329.3634120468287;

	double global_sum = TrapecIntegr(a, b, complex_sqrt_function, N);

	if (rank == 0) {
		ASSERT_LT(std::abs(real_var - global_sum), ERRORRATE);
	}
}

TEST(TrapecIntegral, sincosfun) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int N = 100;
	const double a = -3.141592653589793;
	const double b = 3.141592653589793;
	const double real_var = 4.442882938158356;

	double global_sum = TrapecIntegr(a, b, complex_sin_cos_function, N);

	if (rank == 0) {
		ASSERT_LT(std::abs(real_var - global_sum), ERRORRATE);
	}
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	MPI_Finalize();
	return result;
}