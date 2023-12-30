/* Copyright 2023 Belan Vadim */

#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "task_3/belan_v_optimization_methods/optimization_mpi.h"

TEST(AlgorithmTest, ParetoSetSize) {
    // размерность пространства поиска
    int n = 2;
    // количество итераций
    int k = 100;
    // количество точек в начальном множестве
    int p = 10;
    // точность поиска
    double eps = 1e-6;
    // параметр разброса
    double r = 0.1;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> pareto_set;

    pareto_set = global_search(n, k, p, eps, r);

    if (rank == 0) {
        EXPECT_GT(pareto_set.size(), 0);
    }
}

TEST(AlgorithmTest, ParetoSetDominance) {
    int n = 2;
    int k = 100;
    int p = 10;
    double eps = 1e-6;
    double r = 0.1;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> pareto_set;

    pareto_set = global_search(n, k, p, eps, r);

    if (rank == 0) {
        // Каждая точка в Pareto-множестве не доминирует
        for (size_t i = 0; i < pareto_set.size(); i++) {
            for (size_t j = 0; j < pareto_set.size(); j++) {
                if (i != j) {
                    EXPECT_FALSE(dominates(pareto_set[i], pareto_set[j]));
                }
            }
        }
    }
}

TEST(AlgorithmTest, ObjectiveFunctionValidity) {
    int n = 2;
    int k = 100;
    int p = 10;
    double eps = 1e-6;
    double r = 0.1;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> pareto_set;

    pareto_set = global_search(n, k, p, eps, r);

    if (rank == 0) {
        // Значение целевой функции точки в Pareto-множестве является допустимым
        for (size_t i = 0; i < pareto_set.size(); i++) {
            double f = objective_function(pareto_set[i]);
            EXPECT_GE(f, 0.0);
        }
    }
}

TEST(AlgorithmTest, ParetoSetCoverage) {
    int n = 2;
    int k = 100;
    int p = 10;
    double eps = 1e-6;
    double r = 0.1;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> pareto_set;

    pareto_set = global_search(n, k, p, eps, r);

    if (rank == 0) {
        // Минимальные значения по каждой оси
        std::vector<double> min_vals(n, std::numeric_limits<double>::max());
        // Максимальные значения по каждой оси
        std::vector<double> max_vals(n, std::numeric_limits<double>::lowest());

        // Находим значения по каждой оси
        for (size_t i = 0; i < pareto_set.size(); i++) {
            for (size_t j = 0; j < n; j++) {
                min_vals[j] = std::min(min_vals[j], pareto_set[i][j]);
                max_vals[j] = std::max(max_vals[j], pareto_set[i][j]);
            }
        }

        // Множество Pareto полностью покрывает пространство поиска
        for (size_t j = 0; j < n; j++) {
            EXPECT_LT(min_vals[j], max_vals[j]);
        }
    }
}

TEST(AlgorithmTest, ParetoSetDominanceConvergence) {
    int n = 2;
    int k = 100;
    int p = 10;
    double eps = 1e-6;
    double r = 0.1;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> pareto_set_prev;
    std::vector<std::vector<double>> pareto_set;

    pareto_set_prev = global_search(n, k, p, eps, r);

    for (int i = 0; i < k; i++) {
        pareto_set = global_search(n, 1, p, eps, r);
        if (pareto_set.size() == pareto_set_prev.size()) {
            std::sort(pareto_set.begin(), pareto_set.end());
            std::sort(pareto_set_prev.begin(), pareto_set_prev.end());
            bool convergence = true;
            for (size_t j = 0; j < pareto_set.size(); j++) {
                if (pareto_set[j] != pareto_set_prev[j]) {
                    convergence = false;
                    break;
                }
            }
            if (convergence) {
                break;
            }
        }
        pareto_set_prev = pareto_set;
    }

    if (rank == 0) {
        EXPECT_GT(pareto_set.size(), 0);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
