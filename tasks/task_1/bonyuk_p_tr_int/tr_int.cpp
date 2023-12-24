#include "task_1/bonyuk_p_tr_int/tr_int.h"
#include <cmath>
#include <mpi.h>

double const_function(double x) {
	return 1;
}

double standard_function(double x) {
	return 1 / (1 + x * x);
}

double complex_function(double x) {
	return 3 / (1 + std::pow(x * x + 2 * x + 5, 2));
}

double complex_sqrt_function(double x) {
	return (4 + std::pow(2 * x + 6, 3)) / std::sqrt(x + 2);
}

double complex_sin_cos_function(double x) {
	return (std::pow(cos(x), 3) + 1) / (1 + std::pow(sin(x), 2));
}

double get_area(double a, double b, functional f, int n) {
	double h = (b - a) / n;
	double sum = (f(a) + f(b)) / 2.0;

	for (int i = 1; i < n; ++i) {
		double x = a + i * h;
		sum += f(x);
	}

	return h * sum;
}

double TrapecIntegr(double a, double b, functional f, int N) {
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	const double step = (b - a) / N;
	const int steps_per_proc = N / size;
	const int leftover_steps = N % size;
	double local_a = a + steps_per_proc * step * rank;
	double local_b = local_a + steps_per_proc * step;

	if (rank < leftover_steps) {
		local_a += rank * step;
		local_b += (rank + 1) * step;
	}
	else {
		local_a += leftover_steps * step;
		local_b += leftover_steps * step;
	}

	int local_N = steps_per_proc + (rank < leftover_steps ? 1 : 0);
	double local_area = get_area(local_a, local_b, f, local_N);
	double global_area = 0;

	MPI_Reduce(&local_area, &global_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	return global_area;
}
