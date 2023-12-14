// Copyright 2023 Smirnova Daria
#include <mpi.h>
#include <random>
#include <vector>
#include <ctime>
#include "task_3/smirnova_d_monte_carlo_multiple_integration/monte_carlo_multiple_integration.h"

double Monte_Carlo_Multiple_Integration(double(*f)(std::vector<double>), const std::vector<double>& a,
            const std::vector<double>& b, int n) {
    if (n <= 0) {
        throw "Error: n is negative";
    }

    double res = 0.0;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int multiplicity = a.size();

    double S = 1;
    for (int i = 0; i < multiplicity; i++) {
        S *= (b[i] - a[i]);
    }

    std::vector<std::uniform_real_distribution<double>> r(multiplicity);
    std::vector<double> r1(multiplicity);

    for (int i = 0; i < multiplicity; i++) {
        r[i] = std::uniform_real_distribution<double>(a[i], b[i]);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < multiplicity; j++) {
            r1[j] = r[j](gen);
        }
        res += f(r1);
    }

    res *= S / n;
    return res;
}

double Monte_Carlo_Multiple_Integration_Parallel(double(*f)(std::vector<double>), const std::vector<double>& a,
            const std::vector<double>& b, int n) {
    if (n <= 0) {
        throw "n is negative";
    }

    int rank, size;
    double local_res = 0.0, res = 0.0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int multiplicity = a.size();

    std::vector<std::uniform_real_distribution<double>> r(multiplicity);
    std::vector<double> r1(multiplicity);

    for (int i = 0; i < multiplicity; i++) {
        r[i] = std::uniform_real_distribution<double>(a[i], b[i]);
    }

    for (int i = rank; i < n; i += size) {
        for (int j = 0; j < multiplicity; j++) {
            r1[j] = r[j](gen);
        }
        local_res += f(r1);
    }

    MPI_Reduce(&local_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < multiplicity; i++) {
            res *= (b[i] - a[i]);
        }
        res /= n;
    }

    return res;
}
