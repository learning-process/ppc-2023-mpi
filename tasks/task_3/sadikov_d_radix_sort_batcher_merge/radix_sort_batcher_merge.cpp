// Copyright 2023 Sadikov Damir
#include <vector>
#include <random>
#include <numeric>
#include <cmath>
#include <functional>
#include <utility>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_3/sadikov_d_radix_sort_batcher_merge/radix_sort_batcher_merge.h"

std::vector<double> getRandomVector(int n) {
    std::random_device dev;
    std::mt19937_64 gen(dev());
    std::uniform_real_distribution<double> dist(-10, 10);
    std::vector<double> vec(n);
    for (auto& i : vec) {
        i = dist(gen);
    }
    return vec;
}

std::vector<double> getRandomMatrix(int n) {
    // A[i][i] == 1.0, |A[i][i]| > sum{j!=i}|A[i][j]|
    std::vector<double> ret = getRandomVector(n * n);
    for (int i = 0; i < n; i++) {
        ret[i * n + i] = 1.0;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                ret[i * n + i] += std::abs(ret[i * n + j]);
            }
        }
        for (int j = 0; j < n; j++) {
            if (j != i) {
                ret[i * n + j] /= ret[i * n + i];
            }
        }
        ret[i * n + i] = 1.0;
    }
    return ret;
}

double NormOfDifference(const std::vector<double>& a, const std::vector<double>& b) {
    double ret = 0.0;
    for (int i = 0; i < a.size(); i++) {
        ret += std::abs(a[i] - b[i]);
    }
    return ret;
}

std::vector<double> SequentialIter(const std::vector<double>& A, const std::vector<double>& b, int n) {
    constexpr double epsilon = 1e-5;
    std::vector<double> x(n, 0.0);
    std::vector<double> x_new(n, 0.0);
    bool ok = true;
    while (ok) {
        x_new = b;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    x_new[i] -= A[i * n + j] * x[j];
                }
            }
        }
        ok = NormOfDifference(x, x_new) > epsilon;
        std::swap(x, x_new);
    }
    return x;
}

std::vector<double> ParallelIter(const std::vector<double>& A, const std::vector<double>& b, int n) {
    constexpr double epsilon = 1e-5;
    boost::mpi::communicator world;
    int world_size = world.size();
    int world_rank = world.rank();

    std::vector<int> x_local_sizes(world_size);
    std::vector<int> x_local_displs(world_size);
    for (int i = 0; i < world_size; i++) {
        x_local_sizes[i] = n / world_size + (i < n % world_size ? 1 : 0);
        if (i > 0) {
            x_local_displs[i] = x_local_displs[i - 1] + x_local_sizes[i - 1];
        }
    }

    std::vector<int> A_local_sizes;
    std::vector<int> A_local_displs;
    A_local_sizes.resize(world_size);
    A_local_displs.resize(world_size);
    for (int i = 0; i < world_size; i++) {
        A_local_sizes[i] = n * x_local_sizes[i];
        if (i > 0) {
            A_local_displs[i] = A_local_displs[i - 1] + A_local_sizes[i - 1];
        }
    }

    std::vector<double> A_local(A_local_sizes[world_rank]);
    boost::mpi::scatterv(world, A, A_local_sizes, A_local_displs, A_local.data(), A_local_sizes[world_rank], 0);
    std::vector<double> b_local(A_local_sizes[world_rank]);
    boost::mpi::scatterv(world, b, x_local_sizes, x_local_displs, b_local.data(), x_local_sizes[world_rank], 0);

    std::vector<double> x(n, 0.0);
    std::vector<double> x_new(n, 0.0);
    bool ok = true;
    while (ok) {
        std::vector<double> x_local = b_local;
        for (int i = 0; i < x_local.size(); i++) {
            for (int j = 0; j < n; j++) {
                if (x_local_displs[world_rank] + i != j) {
                    x_local[i] -= A_local[i * n + j] * x[j];
                }
            }
        }

        boost::mpi::all_gatherv(world, x_local, x_new, x_local_sizes, x_local_displs);

        double dif_local = 0.0;
        for (int i = x_local_displs[world_rank]; i < x_local_displs[world_rank] + x_local_sizes[world_rank]; i++) {
            dif_local += std::abs(x[i] - x_new[i]);
        }
        double dif;
        boost::mpi::reduce(world, dif_local, dif, std::plus<double>(), 0);

        if (world_rank == 0) {
            ok = dif > epsilon;
        }

        boost::mpi::broadcast(world, ok, 0);
        swap(x, x_new);
    }
    return x;
}
