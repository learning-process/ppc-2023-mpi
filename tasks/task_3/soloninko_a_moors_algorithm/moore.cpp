// Copyright 2023 Soloninko Andrey
#include "task_3/soloninko_a_moors_algorithm/moore.h"
#include <algorithm>
#include <random>
#include <vector>

#define INF 1000000

std::vector<int> rand_matr(int size) {
    std::vector<int> matrix(size * size);
    std::random_device rd;
    std::uniform_int_distribution<int> unif(1, 100);
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            matrix[i * (size) + j] = unif(rd);
        }
    }
    return matrix;
}

void bellman_moore_parallel(int n, std::vector<int>* mat,
                            std::vector<int>* dist, bool has_negative_cycle) {
    int rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    int loc_n;
    int loc_start, loc_end;

    if (rank == 0) {
        loc_n = n;
    }
    MPI_Bcast(&loc_n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int ave = loc_n / comm_size;
    loc_start = ave * rank;
    loc_end = ave * (rank + 1);
    if (rank == comm_size - 1) {
        loc_end = loc_n;
    }

    std::vector<int> loc_mat(loc_n * loc_n);
    std::vector<int> loc_dist(loc_n);

    if (rank == 0) {
        std::copy((*mat).begin(), (*mat).end(), loc_mat.begin());
    }
    MPI_Bcast(loc_mat.data(), loc_n * loc_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < loc_n; i++) {
        loc_dist[i] = INF;
    }
    loc_dist[0] = 0;

    bool loc_has_change;
    int loc_iter_num = 0;
    for (int iter = 0; iter < loc_n - 1; iter++) {
        loc_has_change = false;
        loc_iter_num++;
        for (int u = loc_start; u < loc_end; u++) {
            for (int v = 0; v < loc_n; v++) {
                int weight = loc_mat[u * loc_n + v];
                if (weight < INF) {
                    if (loc_dist[u] + weight < loc_dist[v]) {
                        loc_dist[v] = loc_dist[u] + weight;
                        loc_has_change = true;
                    }
                }
            }
        }
        MPI_Allreduce(MPI_IN_PLACE, &loc_has_change, 1, MPI_C_BOOL, MPI_LOR,
                      MPI_COMM_WORLD);
        if (!loc_has_change) break;
        MPI_Allreduce(MPI_IN_PLACE, loc_dist.data(), loc_n, MPI_INT, MPI_MIN,
                      MPI_COMM_WORLD);
    }

    if (loc_iter_num == loc_n - 1) {
        loc_has_change = false;
        for (int u = loc_start; u < loc_end; u++) {
            for (int v = 0; v < loc_n; v++) {
                int weight = loc_mat[u * loc_n + v];
                if (weight < INF) {
                    if (loc_dist[u] + weight < loc_dist[v]) {
                        loc_dist[v] = loc_dist[u] + weight;
                        loc_has_change = true;
                        break;
                    }
                }
            }
        }
        MPI_Allreduce(&loc_has_change, &has_negative_cycle, 1, MPI_C_BOOL,
                      MPI_LOR, MPI_COMM_WORLD);
    }

    if (rank == 0) std::copy(loc_dist.begin(), loc_dist.end(), (*dist).begin());
}

void bellman_moore(int n, std::vector<int>* mat, std::vector<int>* dist,
                   bool has_negative_cycle) {
    has_negative_cycle = false;
    for (int i = 0; i < n; i++) {
        (*dist)[i] = INF;
    }
    (*dist)[0] = 0;

    bool has_change;
    for (int i = 0; i < n - 1; i++) {
        has_change = false;
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                int weight = (*mat)[u * n + v];
                if (weight < INF) {
                    if ((*dist)[u] + weight < (*dist)[v]) {
                        has_change = true;
                        (*dist)[v] = (*dist)[u] + weight;
                    }
                }
            }
        }
        if (!has_change) {
            return;
        }
    }
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            int weight = (*mat)[u * n + v];
            if (weight < INF) {
                if ((*dist)[u] + weight < (*dist)[v]) {
                    has_negative_cycle = true;
                    return;
                }
            }
        }
    }
}
