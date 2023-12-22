// Copyright 2023 Musaev Ilgar

#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include "task_1/musaev_i_integr_method_monte_carlo/method_monte_carlo.h"

double monteCarloIntegration(double min_lim, double max_lim, double(*pfunc)(double), int dots_count) {
    if (dots_count < 0)
        throw(1);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::mt19937 gen;
    time_t curr_time = static_cast<time_t>(rank);
    gen.seed(static_cast<unsigned int>(time(&curr_time)));
    std::uniform_real_distribution<> urd(0, 1);

    double delta = (max_lim - min_lim) / size;
    int local_count = dots_count / size;
    double local_sum = 0.0;
    double global_sum = 0.0;
    for (int i = 0; i < local_count; i++) {
        local_sum += pfunc(min_lim + delta * rank + urd(gen)*delta);
    }
    local_sum = local_sum * delta / local_count;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}
