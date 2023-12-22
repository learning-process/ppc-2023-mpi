// Copyright 2023 Khodyrev Fedor

#include <mpi.h>
#include <random>
#include <functional>
#include <cstdlib>
#include "task_1/khodyrev_f_monte_carlo_method/monte_carlo_method.h"

double seq_monte_carlo_integration(int num_points, double lower_limit,
double upper_limit, std::function<double(double)> func) {
    int points_inside = 0;
    for (int i = 0; i < num_points; i++) {
        double x = lower_limit + (upper_limit - lower_limit) *
        std::rand() / RAND_MAX;
        double y = func(x);
        if (y > static_cast<double>(std::rand()) / RAND_MAX) {
            points_inside++;
        }
    }
    double area = (upper_limit - lower_limit) * 1.0 *
    points_inside / num_points;
    return area;
}

double par_monte_carlo_integration(int num_points, double lower_limit,
double upper_limit, std::function<double(double)> func) {
    int points_inside = 0;
    int size_world = 0;
    int rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int part_of_points = num_points / size_world;
    MPI_Bcast(&part_of_points, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int local_points_inside = 0;
    for (int i = 0; i < part_of_points; i++) {
        double x = lower_limit + (upper_limit - lower_limit)
        * std::rand() / RAND_MAX;
        double y = func(x);
        if (y > static_cast<double>(std::rand()) / RAND_MAX) {
            local_points_inside++;
        }
    }
    MPI_Reduce(&local_points_inside, &points_inside, 1, MPI_INT,
    MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        double area = (upper_limit - lower_limit) * 1.0 *
        points_inside / num_points;
        return area;
    }
    return 0.0;
}
