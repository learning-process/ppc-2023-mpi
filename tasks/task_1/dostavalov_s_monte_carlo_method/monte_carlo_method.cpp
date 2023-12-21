// Copyright 2023 Dostavalov Semen
#include <mpi.h>
#include <random>
#include <functional>
#include <cstdlib>
#include "task_1/dostavalov_s_monte_carlo_method/monte_carlo_method.h"


double monteCarlo(int low, int high, int count, double (*f)(double)) {
    std::random_device dev;
    std::mt19937 gen(dev());

    std::uniform_real_distribution<> distx(low, high);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int delta = count / size;
    int ibeg = rank * delta + 1;
    int iend = (rank + 1) * delta;

    double res;
    float x, y;
    double iter, res1;
    double d = f(low);

    for (iter = low; iter <= high; iter += (high - low) / 500.0) {
        if (f(iter) > d)
            d = f(iter);
    }
    std::uniform_real_distribution<> disty(0, d);

    MPI_Bcast(&count, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double sum = 0;

    for (int i = ibeg; i < iend; i++) {
        x = distx(gen);
        y = disty(gen);
        if (y <= f(x)) {
            sum++;
        }
    }
    double localSum = sum * (high - low) * d / (iend - ibeg);
    MPI_Reduce(&localSum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    res = localSum;
    return res;
}


