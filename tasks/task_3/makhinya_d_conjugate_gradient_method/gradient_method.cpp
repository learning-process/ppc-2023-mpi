// Copyright 2023 Makhinya Danil
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_3/makhinya_d_conjugate_gradient_method/gradient_method.h"


double dot(const vector& a, const vector& b) {
    double res = 0.0;
    uint16_t sz = b.size();
    for (int i = 0; i < sz; ++i) {
        res += a[i] * b[i];
    }
    return res;
}

vector operator* (const matrix& A, const vector& b) {
    uint16_t sz = b.size();
    vector res(sz);
    for (int i = 0; i < sz; ++i) {
        res[i] = dot(A[i], b);
    }
    return res;
}

vector operator* (double c, const vector& b) {
    vector res(b);
    uint16_t sz = b.size();
    for (int i = 0; i < sz; ++i) {
        res[i] *= c;
    }
    return res;
}

vector operator+ (const vector& a, const vector& b) {
    uint16_t sz = b.size();
    vector res(sz);
    for (int i = 0; i  < sz; ++i) {
        res[i] = a[i] + b[i];
    }
    return res;
}

vector operator- (const vector& a, const vector& b) {
    return a + (-1.0 * b);
}

double measure(const vector& r) {
    double res = 0.0;
    for (double x : r) {
        res = std::max(res, std::abs(x));
    }
    return res;
}

vector solve_parallel(const matrix& A, const vector& b) {
    boost::mpi::communicator world;
    auto sz = b.size();
    auto world_sz = world.size();
    auto delta = sz / world_sz;
    auto tail =  sz % world_sz;
    vector x;

    int usefull_ptr[10] = {0};
    if (world.rank() == 0) {
        vector x_prev(sz);
        vector r_prev = b - A * x_prev;
        vector z_prev = r_prev;
        int count_iter = 0;
        while (measure(r_prev) > EPS && count_iter < MAX_COUNT_ITER) {
            double dot_r_prev = dot(r_prev, r_prev);
            vector tmp(sz);
            for (int i = 0; i < sz; ++i) {
                tmp[i] = dot(A[i], z_prev);
            }
            double alpha = dot_r_prev / dot(tmp, z_prev);
            r_prev = r_prev - alpha * tmp;
            double betta = dot(r_prev, r_prev) / dot_r_prev;

            x_prev = x_prev + alpha * z_prev;
            z_prev = r_prev + betta * z_prev;

            count_iter++;
        }

        for (int proc = 1; proc < world_sz; ++proc) {
            world.send(proc, 0, usefull_ptr, 1);
        }
        x = x_prev;
    } else {
        world.recv(0, 0, usefull_ptr, 1);
    }

    return x;
}
