// Copyright 2023 Sobol Lubov

#include "task_3/sobol_l_simpson_method/simpson_method.h"

double simpsonMethod(double start, double end, double lower,
                     double upper, int intervals) {
    if (intervals % 2 != 0) {
        intervals++;
    }
    double h1 = (end - start) / (intervals);
    double h2 = (upper - lower) / (intervals);
    double sum = 0.0;
    double coef1 = 0.0;
    double coef2 = 0.0;

    for (int i = 0; i <= intervals; i++) {
        double x = start + i * h1;
        for (int j = 0; j <= intervals; j++) {
            double y = lower + j * h2;
            if (i == 0 || i == intervals) {
                coef1 = 1.0;
            } else {
                if (i % 2 == 0) {
                    coef1 = 2.0;
                } else {
                    coef1 = 4.0;
                }
            }
            if (j == 0 || j == intervals) {
                coef2 = 1.0;
            } else {
                if (j % 2 == 0) {
                    coef2 = 2.0;
                } else {
                    coef2 = 4.0;
                }
            }
            coef1 = coef1 * coef2;
            sum += coef1 * x * y;
        }
    }

    return (h1 * h2 / 9.0) * sum;
}

double parallelSimpsonMethod(double start, double end, double lower,
                             double upper, int intervals) {
    MPI_Comm comm = MPI_COMM_WORLD;

    int num_procs;
    MPI_Comm_size(comm, &num_procs);
    int rank;
    MPI_Comm_rank(comm, &rank);

    if (intervals % 2 != 0) {
        intervals++;
    }
    int local_intervals = intervals / num_procs;
    int remain = intervals % num_procs;
    if (rank == 0) {
        local_intervals += remain;
    }
    double h1 = (end - start) / (intervals);
    double h2 = (upper - lower) / (intervals);
    double local_sum = 0.0;

    // Calculate local start and end indices
    int local_start = rank * local_intervals + (rank > remain ? remain : rank);
    int local_end = local_start + local_intervals - 1;

    // Calculate local integration using the sequential function for the local data
    local_sum = simpsonMethod(start + local_start * h1, start + local_end * h1, lower, upper, local_intervals);

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

    if (rank == 0) {
        return (h1 * h2) / (9.0) * global_sum;
    } else {
        return 0.0;
    }
}
