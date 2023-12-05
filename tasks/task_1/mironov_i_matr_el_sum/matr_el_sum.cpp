// Copyright 2023 Mironov Ilya
#include <random>
#include <numeric>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/mironov_i_matr_el_sum/matr_el_sum.h"

std::vector<int> get_random_matrix(int sz, int a, int b) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matr(sz);
    for (int  i = 0; i < sz; i++) { matr[i] = gen() % (b - a + 1) + a; }
    return matr;
}

int get_sequential_matr_el_sum(const std::vector<int> &matr) {
    return std::accumulate(matr.begin(), matr.end(), 0);
}

int get_parallel_matr_el_sum(const std::vector<int> &matr, int sz) {
    boost::mpi::communicator world;
    int local_size = sz / world.size();
    int remainder = sz % world.size();
    int local_sum;
    std::vector<int> sizes(world.size(), local_size);
    for (int i = 0; i < remainder; i++) {
        sizes[i]++;
    }
    std::vector<int> local_vec(sizes[world.rank()]);
    boost::mpi::scatterv(world, matr, sizes, local_vec.data(), 0);
    if (world.rank() == 0) {
        local_vec = std::vector<int>(matr.begin(), matr.begin() + sizes[0]);
    }

    if (local_vec.size() > 0) {
        local_sum = std::accumulate(local_vec.begin(), local_vec.end(), 0);
    } else {
        local_sum = 0;
    }
    int global_sum;
    boost::mpi::reduce(world, local_sum, global_sum, std::plus<int>(), 0);
    return global_sum;
}
