// Copyright 2023 Lysanova Julia
#include <random>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/lysanova_j_max_elem/max_elem.h"



std::vector<int> RandomVector(int sz, int a, int b) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int i = 0; i < sz; i++) {
        vec[i] = gen() % (b - a + 1) + a;
    }
    return vec;
}



int MaxElem(std::vector<int> vec, int sz) {
    boost::mpi::communicator world;
    int res = INT_MIN;
    int local_max = res;
    int np = world.size();
    int chunk_sz = sz / np, remainder = sz % np;
    std::vector<int> sizes(np, chunk_sz);

    for (int i = 0; i < remainder; i++) sizes[i]++;

    std::vector<int> local_vec(sizes[world.rank()]);
    boost::mpi::scatterv(world, vec, sizes, local_vec.data(), 0);

    if (local_vec.size())
        local_max = *std::max_element(local_vec.begin(), local_vec.end());

    boost::mpi::reduce(world, local_max, res, boost::mpi::maximum<int>(), 0);

    return std::max(res, local_max);
}
