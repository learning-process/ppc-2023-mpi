// Copyright 2023 Petrov Maksim
#include <random>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/petrov_m_simple_iteration_method/simple_iteration_method.h"

using std::vector;
using std::mt19937_64;
using std::random_device;
using std::uniform_real_distribution;
using std::abs;
using std::max;
using std::memcpy;

vector<double> get_rand_vec(int n) {
    vector<double> vec(n);
    mt19937_64 generator{ random_device {}() };
    uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < n; ++i)
        vec[i] = distribution(generator);

    return vec;
}

vector<double> get_rand_matrix(int n) {
    vector<double> matrix(n * n);
    double* data = matrix.data();
    double diag_elem;
    mt19937_64 generator{ random_device {}() };
    uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        diag_elem = 0.0;
        for (int j = 0; j < n; ++j) {
            data[i * n + j] = distribution(generator);
            diag_elem += data[i * n + j];
        }
        data[i * (n + 1)] = 1.5 * diag_elem;
    }

    return matrix;
}

vector<double> get_sequential_simple_iteration_method(const vector<double>& A_authentic,
    const vector<double>& b_authentic, int n, int max_numb_iter, double eps) {
    vector<double> x_final(n);
    const double* A;
    const double* b;
    double* x_cur;
    double* A_copy;
    double* b_copy;
    double* x_new;

    A = A_authentic.data();
    b = b_authentic.data();
    x_cur = x_final.data();
    A_copy = new double[n * n];
    b_copy = new double[n];
    x_new = new double[n];

    memcpy(A_copy, A, n * n * sizeof(double));
    memcpy(b_copy, b, n * sizeof(double));

    double diff;

    for (int i = 0; i < n; ++i) {
        double diag_elem = A_copy[(n + 1) * i];
        A_copy[(n + 1) * i] = 0.0;
        b_copy[i] /= diag_elem;
        for (int j = 0; j < n; ++j)
            A_copy[i * n + j] /= diag_elem;
    }

    for (int iter = 0; iter < max_numb_iter; ++iter) {
        for (int i = 0; i < n; ++i) {
            x_new[i] = b_copy[i];
            for (int j = 0; j < n; ++j)
                x_new[i] -= A_copy[i * n + j] * x_cur[j];
        }
        diff = 0.0;
        for (int i = 0; i < n; ++i)
            diff = max(diff, abs(x_new[i] - x_cur[i]));
        if (diff < eps)
            break;
        memcpy(x_cur, x_new, n * sizeof(double));
    }

    delete[] A_copy;
    delete[] b_copy;
    delete[] x_new;
    return x_final;
}

vector<double> get_parallel_simple_iteration_method(const vector<double>& A_vector,
    const vector<double>& b_vector, int n, int max_numb_iter, double epsilon) {
    vector<double> x_final(n);

    int block_begin;
    int block_size;
    double* A_local_row;
    double* b_local;
    double* x_cur;
    double* x_local;
    double local_diff;
    double total_diff;

    boost::mpi::communicator world;
    int size = world.size();
    int rank = world.rank();

    block_begin = rank * n / size;
    block_size = ((rank + 1) * n) / size - (rank * n) / size;
    A_local_row = new double[block_size * n];
    b_local = new double[block_size];
    x_local = new double[block_size];
    x_cur = x_final.data();

    if (rank == 0) {
        int pos_begin;
        int pos_end;

        const double* A = A_vector.data();
        const double* b = b_vector.data();

        memcpy(A_local_row, A, block_size * n * sizeof(double));
        memcpy(b_local, b, block_size * sizeof(double));

        for (int i = 1; i < size; ++i) {
            pos_begin = i * n / size;
            pos_end = (i + 1) * n / size;

            world.send(i, 0, A + pos_begin * n, (pos_end - pos_begin) * n);
            world.send(i, 1, b + pos_begin, pos_end - pos_begin);
        }
    } else {
        world.recv(0, 0, A_local_row, block_size * n);
        world.recv(0, 1, b_local, block_size);
    }

    for (int i = 0; i < block_size; ++i) {
        double diag_elem = A_local_row[(n + 1) * i + block_begin];
        A_local_row[(n + 1) * i + block_begin] = 0.0;
        b_local[i] /= diag_elem;
        for (int j = 0; j < n; ++j)
            A_local_row[i * n + j] /= diag_elem;
    }

    for (int iter = 0; iter < max_numb_iter; ++iter) {
        for (int i = 0; i < block_size; ++i) {
            x_local[i] = b_local[i];
            for (int j = 0; j < n; ++j)
                x_local[i] -= A_local_row[i * n + j] * x_cur[j];
        }

        local_diff = 0.0;

        for (int i = 0; i < block_size; ++i)
            local_diff = max(local_diff, abs(x_local[i] - x_cur[block_begin + i]));

        world.barrier();

        reduce(world, &local_diff, 1, &total_diff, boost::mpi::maximum<double>(), 0);

        broadcast(world, &total_diff, 1, 0);

        if (total_diff < epsilon)
                break;

        if (rank == 0) {
            memcpy(x_cur, x_local, block_size * sizeof(double));
            for (int i = 1; i < size; ++i) {
                world.recv(i, 2, x_cur + i * n / size, (i + 1) * n / size - i * n / size);
            }
        } else {
            world.send(0, 2, x_local, block_size);
        }
        broadcast(world, x_cur, n, 0);
    }

    delete[] A_local_row;
    delete[] b_local;
    delete[] x_local;
    return x_final;
}

vector<double> mult_for_verification_aprox(const vector<double>& A_vector, const vector<double>& x_vector, int n) {
    vector<double> b_vector(n);
    const double* A = A_vector.data();
    const double* x = x_vector.data();
    double* b = b_vector.data();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            b[i] += A[i * n + j] * x[j];
    }

    return b_vector;
}
