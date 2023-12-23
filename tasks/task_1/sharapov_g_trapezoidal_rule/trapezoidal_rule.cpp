// Copyright 2023 Sharapov Georgiy
#include <mpi.h>
#include <cmath>
#include <functional>
#include "task_1/sharapov_g_trapezoidal_rule/trapezoidal_rule.h"

double integralTrapezoidalParallel(double a, double b, int64_t n, std::function<double(double)> func) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double stepSize = (b - a) / n;
    double localSum = 0.0;
    for (int i = ProcRank; i < n; i += ProcNum) {
        double Xi = a + i * stepSize;
        double Xj = a + (i + 1) * stepSize;
        localSum += ((func(Xi) + func(Xj)) / 2) * stepSize;
    }

    double globalSum = 0.0;
    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return globalSum;
}
