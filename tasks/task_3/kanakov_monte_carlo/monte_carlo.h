// Copyright 2023 Kanakov Roman
#ifndef TASKS_TASK_3_KANAKOV_MONTE_CARLO_MONTE_CARLO_H_
#define TASKS_TASK_3_KANAKOV_MONTE_CARLO_MONTE_CARLO_H_
#include <mpi.h>
#include <cstdint>
#include <array>
#include <functional>
#include <random>

template <size_t dim>
double multiple_integral_monte_carlo_parallel(
        const std::function<double(std::array<double, dim>)> &f,
        const std::function<bool(std::array<double, dim>)> &in_region,
        const std::array<double[2], dim> &rect,
        int64_t n) {
    int rank;
    int size;

    int64_t n_per_process;

    double local_sum = 0.0;
    double total_sum = 0.0;

    double multiplier = 1/static_cast<double>(n);

    double current_value;
    double corrected_value;
    double error_sum = 0.0;
    double corrected_sum;

    std::array<double, dim> random_point;
    std::mt19937_64 generator{std::random_device {}()};
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < dim; ++i)

        multiplier *= (rect[i][1] - rect[i][0]);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    n_per_process = ((rank + 1) * n) / size - (rank * n) / size;

    for (int64_t it = 0; it < n_per_process; ++it) {
        for (size_t i = 0; i < dim; ++i)
            random_point[i] = rect[i][0] +
            (rect[i][1] - rect[i][0]) * distribution(generator);

        if (in_region(random_point)) {
            current_value = multiplier * f(random_point);
            corrected_value = current_value - error_sum;
            corrected_sum = local_sum + corrected_value;

            error_sum = (corrected_sum - local_sum) - corrected_value;
            local_sum = corrected_sum;
        }
    }
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);
    return total_sum;
}

template <size_t dim>
double multiple_integral_monte_carlo_sequential(
        const std::function<double(std::array<double, dim>)> &f,
        const std::function<bool(std::array<double, dim>)> &in_region,
        const std::array<double[2], dim> &rect,
        int64_t n) {
    double total_sum = 0.0;
    double multiplier = 1/static_cast<double>(n);
    double current_value;
    double corrected_value;
    double error_sum = 0.0;
    double corrected_sum;

    std::array<double, dim> random_point;
    std::mt19937_64 generator{std::random_device {}()};
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < dim; ++i)
        multiplier *= (rect[i][1] - rect[i][0]);

    for (int64_t it = 0; it < n; ++it) {
        for (size_t i = 0; i < dim; ++i)
            random_point[i] = rect[i][0] +
            (rect[i][1] - rect[i][0]) * distribution(generator);
        if (in_region(random_point)) {
            current_value = multiplier * f(random_point);
            corrected_value = current_value - error_sum;
            corrected_sum = total_sum + corrected_value;
            error_sum = (corrected_sum - total_sum) - corrected_value;
            total_sum = corrected_sum;
        }
    }

    return total_sum;
}
#endif  // TASKS_TASK_3_KANAKOV_MONTE_CARLO_MONTE_CARLO_H_
