// Copyright 2023 Kozlov Mikhail
#include <random>
#include <iostream>
#include "task_2/kozlov_m_seidel_method/seidel_method.h"


std::vector<double> seidel_parallel(std::vector<std::vector<double>> A, std::vector<double> B, int n) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int localsize = n / size;
    int l = localsize * rank;
    int r = localsize * (rank + 1);

    if (rank == size - 1 && localsize != 0) {
        r = n;
    }
    if (rank == 0 && localsize == 0) {
        r = n;
    }

    double localSum = 0.0, globalSum = 0.0, diff = 0.0;

    std::vector<double> x(n, 0.0);
    std::vector<double> prev_x(n, 0.0);

    do {
        for (int i = 0; i < n; i++) {
            prev_x[i] = x[i];
        }

        for (int i = 0; i < n; i++) {
            localSum = 0.0;
            globalSum = 0.0;

            for (int j = l; j < r; j++) {
                if ((0 <= j) && (j < i)) {
                    localSum += (A[i][j] * x[j]);
                } else if ((i + 1 <= j) && (j < n)) {
                    localSum += (A[i][j] * prev_x[j]);
                }
            }

            MPI_Allreduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

            x[i] = (B[i] - globalSum) / A[i][i];
        }

        double local_diff = 0.0;
        diff = 0.0;
        for (int i = l; i < r; i++) {
            local_diff += std::pow(x[i] - prev_x[i], 2);
        }
        MPI_Allreduce(&local_diff, &diff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    } while (!(std::sqrt(diff) < 1e-3));

    return x;
}

std::vector<double> seidel_seq(std::vector<std::vector<double>> A, std::vector<double> B, int n) {
    std::vector<double> x(n);
    std::vector<double> prev_x(n);

    double diff = 0;
    do {
        for (int i = 0; i < n; i++) {
            prev_x[i] = x[i];
        }

        for (int i = 0; i < n; i++) {
            double var = 0;

            for (int j = 0; j < i; j++) {
                var += (A[i][j] * x[j]);
            }

            for (int j = i + 1; j < n; j++) {
                var += (A[i][j] * prev_x[j]);
            }

            x[i] = (B[i] - var) / A[i][i];
        }

        diff = 0;
        for (int i = 0; i < n; i++) {
            diff += std::pow(x[i] - prev_x[i], 2);
        }
    } while (!(std::sqrt(diff) < 1e-3));

    return x;
}