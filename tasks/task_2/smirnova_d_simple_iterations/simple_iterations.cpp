// Copyright 2023 Smirnova Daria
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include "task_2/smirnova_d_simple_iterations/simple_iterations.h"

std::vector<double> generate_random_Matrix(int size) {
    if (size < 2)
        throw "Incorrect size of matrix";

    std::mt19937 gen;
    gen.seed(static_cast<double>(time(NULL)));

    std::vector<double> matrix(size * (size + 1));
    for (int i = 0; i < size * (size + 1); i++) {
        matrix[i] = gen() % 20;
    }

    return matrix;
}

bool is_equal(std::vector<double> x, std::vector<double> y) {
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
        if (!(std::fabs(x[i] - y[i]) < 1e-4))
            return false;
    }
    return true;
}

std::vector<double> simple_iterations(std::vector<double> delta_a, std::vector<double> x,
    double error, int size, int rank, int row_count, int size_proc) {
    std::vector<double> x_old(size);
    std::vector<double> temp(size);
    std::vector<int> sendcounts(size_proc);
    std::vector<int> displs(size_proc);
    int core;
    double norm = 0, val;

    MPI_Scan(&row_count, &core, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    core -= row_count;

    MPI_Allgather(&row_count, 1, MPI_INT, &sendcounts[0], 1, MPI_INT, MPI_COMM_WORLD);

    displs[0] = 0;
    for (int i = 1; i < size_proc; i++) {
        displs[i] = displs[i - 1] + sendcounts[i - 1];
    }

    do {
        x_old = x;
        double sum;

        for (int i = 0; i < row_count; i++) {
            sum = 0.0;
            for (int j = 0; j < i + core; j++) {
                sum += delta_a[i * (size + 1) + j] * x_old[j];
            }

            for (int j = 1 + core + i; j < size; j++) {
                sum += delta_a[i * (size + 1) + j] * x_old[j];
            }
            x[i + core] = static_cast<double>(delta_a[i * (size + 1) + size] - sum) /
                static_cast<double>(delta_a[i * (size + 1) + i + core]);
        }

        MPI_Allgatherv(&x[0] + core, row_count, MPI_DOUBLE, &temp[0],
            &sendcounts[0], &displs[0], MPI_DOUBLE, MPI_COMM_WORLD);

        x = temp;
        norm = 0;
        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                val = fabs(x[i] - x_old[i]);
                if (norm < val) norm = val;
            }
        }

        MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } while (error < norm);

    return x;
}

std::vector<double> get_result(std::vector<double> matrix, int size, double error) {
    if (size * (size + 1) != static_cast<int>(matrix.size())) {
        throw "Non-equal dimensions";
    }

    std::vector<double> MATRIX(size);
    int size_proc, rank, m_size;
    MPI_Comm_size(MPI_COMM_WORLD, &size_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int row_count, SIZE, eps;
    if (rank == 0) {
        m_size = size;
        eps = error;
        MATRIX = std::vector<double>(size * (size + 1));
        for (int i = 0; i < size * (size + 1); i++) {
            MATRIX[i] = matrix[i];
        }
    }

    MPI_Bcast(&m_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> x(m_size);
    MPI_Bcast(&x[0], m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    row_count = (m_size / size_proc) + ((m_size % size_proc) > rank ? 1 : 0);
    SIZE = (m_size + 1) * row_count;

    std::vector<int> sendcounts(size_proc);
    MPI_Gather(&SIZE, 1, MPI_INT, &sendcounts[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> displs(size_proc);
    displs[0] = 0;
    for (int i = 1; i < size_proc; i++) {
        displs[i] = displs[i - 1] + sendcounts[i - 1];
    }

    std::vector<double> delta_a((m_size + 1) * row_count + (row_count > 0 ? 0 : 1));
    MPI_Scatterv(&MATRIX[0], &sendcounts[0], &displs[0], MPI_DOUBLE, &delta_a[0], SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    x = simple_iterations(delta_a, x, error, size, rank, row_count, size_proc);

    return x;
}
