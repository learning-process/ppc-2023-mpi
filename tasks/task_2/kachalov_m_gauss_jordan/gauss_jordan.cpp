// Copyright 2023 Kachalov Mikhail
#include "task_2/kachalov_m_gauss_jordan/gauss_jordan.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::vector<double> solve_linear_system_sequential(
    const std::vector<std::vector<double>>& A, const std::vector<double>& b) {
    int n = A.size();
    std::vector<std::vector<double>> augmented_matrix(n, std::vector<double>(n + 1, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented_matrix[i][j] = A[i][j];
        }
        augmented_matrix[i][n] = b[i];
    }

    for (int i = 0; i < n; ++i) {
        double pivot = augmented_matrix[i][i];
        for (int j = i; j <= n; ++j) {
            augmented_matrix[i][j] /= pivot;
        }

        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented_matrix[k][i];
                for (int j = i; j <= n; ++j) {
                    augmented_matrix[k][j] -= factor * augmented_matrix[i][j];
                }
            }
        }
    }

    std::vector<double> solution(n);
    for (int i = 0; i < n; ++i) {
        solution[i] = augmented_matrix[i][n];
    }

    return solution;
}

std::vector<double> solve_linear_system_parallel(
    const std::vector<std::vector<double>>& A, const std::vector<double>& b) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int n = A.size();
    std::vector<std::vector<double>> augmented_matrix(n, std::vector<double>(n + 1, 0.0));
    const int delta = n / world.size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented_matrix[i][j] = A[i][j];
        }
        augmented_matrix[i][n] = b[i];
    }

    if (world.rank() == 0) {
        for (int proc = 1; proc < world.size(); proc++) {
            boost::mpi::scatter(
                world, A[proc * delta], delta, augmented_matrix[proc * delta], 0);
            boost::mpi::scatter(
                world, b.begin() + proc * delta, b.begin() + (proc + 1) * delta,
                augmented_matrix[proc * delta].begin() + n, 0);
        }
    } else {
        boost::mpi::scatter(
            world, A[0], delta, augmented_matrix[0], 0);
        boost::mpi::scatter(
            world, b.begin(), b.begin() + delta, augmented_matrix[0].begin() + n, 0);
    }

    for (int i = 0; i < delta; ++i) {
        int row = i + world.rank() * delta;

        double pivot = augmented_matrix[i][i];
        for (int j = i; j <= n; ++j) {
            augmented_matrix[i][j] /= pivot;
        }

        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented_matrix[k][i];
                for (int j = i; j <= n; ++j) {
                    augmented_matrix[k][j] -= factor * augmented_matrix[i][j];
                }
            }
        }
    }

    if (world.rank() == 0) {
        for (int proc = 1; proc < world.size(); proc++) {
            boost::mpi::gather(
                world, augmented_matrix[proc * delta], delta * (n + 1),
                augmented_matrix[proc * delta], 0);
        }
    } else {
        boost::mpi::gather(
            world, augmented_matrix[0], delta * (n + 1),
            augmented_matrix[0], 0);
    }

    std::vector<double> solution(n);
    for (int i = 0; i < n; ++i) {
        solution[i] = augmented_matrix[i][n];
    }

    return solution;
}

std::vector<std::vector<double>> generate_random_matrix(const int& rows, const int& cols) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0.0));
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = gen() % 100;
        }
    }

    return matrix;
}
