// Copyright 2023 Gontsov Roman
#include <mpi.h>
#include <cmath>
#include <functional>
#include "task_1/gontsov_r_integrate_trapez/integrate_trapez.h"

double getSequentialIntegration(const std::function<double(double)>& f,
    double a, double b, int n) {
    double sum = 0;
    double interval = (b - a) / static_cast<double>(n);
    for (int i = 0; i < n; i++)
        sum += ((f(a + static_cast<double>(i + 1) * interval)
            + f(a + static_cast<double>(i) * interval)) * 0.5);
    sum *= interval;
    return sum;
}

double getParallelIntegration(const std::function<double(double)>& f,
    double a, double b, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size == 1 || n < size)
        return getSequentialIntegration(f, a, b, n);
    double interval = (b - a) / static_cast<double>(n);
    int count_interv = n / size;
    double local_integral = 0, global_integral = 0;
    double begin_interval = a + static_cast<double>(rank) *
        interval * static_cast<double>(count_interv);
    double end_interval = begin_interval + interval;
    for (int i = 0; i < count_interv; i++) {
        local_integral += ((f(begin_interval) + f(end_interval)) * 0.5);
        begin_interval += interval;
        end_interval += interval;
    }
    local_integral *= interval;
    MPI_Reduce(&local_integral, &global_integral, 1,
        MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if ((rank == 0) && (n % size != 0)) {
        double rem = getSequentialIntegration(f, b - (n % size)
            * interval, b, n % size);
        global_integral += rem;
    }
    return global_integral;
}
