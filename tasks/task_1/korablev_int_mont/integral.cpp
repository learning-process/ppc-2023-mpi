// Copyright 2023 Korablev Nikita
#include <cmath>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/korablev_int_mont/integral.h"

double fsin(double x) { return sin(x); }
double fcos(double x) { return cos(x); }
double f_two_sin_cos(double x) { return 2*sin(x)*cos(x); }

double integral(double a, double b, int N, func f) {
    boost::mpi::communicator world;
    double h = (b - a) / static_cast<double>(N);
    double sum = 0.0;

    for (int i = 0; i < N; ++i) {
        sum += f(a + h*i);
    }

    return h*sum;
}

double parallel_integral(double a, double b, int N, func f) {
    boost::mpi::communicator world;
    double chunk = (b - a) / world.size();

    double a_chunk = a + chunk * world.rank();
    double b_chunk = a_chunk + chunk;
    double h_chunk = (b_chunk - a_chunk) / N;

    double local_sum = 0.0;
    double global_sum = 0.0;

    for (int i = 0; i < N; ++i) { local_sum += f(a_chunk + h_chunk * i); }
    local_sum *= h_chunk;

    reduce(world, local_sum, global_sum, std::plus<double>(), 0);
    return global_sum;
}



