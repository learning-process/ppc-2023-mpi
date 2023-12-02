// Copyright 2023 Ivanov Nikita
#include <cmath>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/ivanov_int_tr/int_tr.h"

double sin_f(double x) { return sin(x); }
double sin2_f(double x) { return sin(x) / (1 - std::pow(sin(x), 2)); }
double hardfn_f(double x) { return 3*x / std::sqrt(std::pow(x + 1, 3)); }
double hardfn2_f(double x) { return (std::pow(x, 2) + 2 * x - 3) / std::pow(x, 4); }
double sin_cos_f(double x) { return sin(x) * std::pow(cos(x), 2); }

double trapezium(double a, double b, func f) { return (f(a)+f(b))*std::abs(b-a)/2; }

double get_area(double a, func f, int steps_count, double step) {
    double res = 0;
    for (int i = 0; i < steps_count; i++) {
        res += trapezium(a + i * step, a + (i + 1) * step, f);
    }
    return res;
}

double getParallelOperations(double a, double b, int N, func f) {
    boost::mpi::communicator world;
    const double step = std::abs(b-a) / N;
    const int steps_count = N / world.size();

    if (world.rank() == 0) {
        for (int proc = 1; proc < world.size(); proc++) {
            world.send(proc, 0, a + steps_count * step * proc);
        }
    }

    double local_val;
    if (world.rank() == 0) {
        local_val = a;
    } else {
        world.recv(0, 0, local_val);
    }
    double global_area = 0;
    double local_area = 0;

    if (world.rank() == world.size() - 1) {
        local_area = get_area(local_val, f, steps_count + N - steps_count * world.size(), step);
    } else {
        local_area = get_area(local_val, f, steps_count, step);
    }
    reduce(world, local_area, global_area, std::plus<double>(), 0);
    return global_area;
}
