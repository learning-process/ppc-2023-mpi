// Copyright 2023 Sadikov Damir
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_1/sadikov_d_dot_product/dot_product.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> dist(-100, 100);
    std::vector<int> vec(sz);
    for (auto& i : vec) {
        i = dist(gen);
    }
    return vec;
}

int getSequentialDotProduct(const std::vector<int>& A, const std::vector<int>& B) {
    return std::inner_product(A.begin(), A.end(), B.begin(), 0);
}

int getParallelDotProduct(const std::vector<int>& A,
                          const std::vector<int>& B, int vector_size) {
    boost::mpi::communicator world;
    const int delta = vector_size / world.size() + (world.rank() < vector_size % world.size() ? 1 : 0);

    if (world.rank() == 0) {
        int off = delta;
        for (int proc = 1; proc < world.size(); proc++) {
            int proc_delta = vector_size / world.size() + (proc < vector_size % world.size() ? 1 : 0);
            world.send(proc, 0, A.data() + off, proc_delta);
            world.send(proc, 1, B.data() + off, proc_delta);
            off += proc_delta;
        }
    }

    std::vector<int> local_A(delta);
    std::vector<int> local_B(delta);
    if (world.rank() == 0) {
        local_A = std::vector<int>(A.begin(), A.begin() + delta);
        local_B = std::vector<int>(B.begin(), B.begin() + delta);
    } else {
        world.recv(0, 0, local_A.data(), delta);
        world.recv(0, 1, local_B.data(), delta);
    }

    int global_sum = 0;
    int local_sum = getSequentialDotProduct(local_A, local_B);
    reduce(world, local_sum, global_sum, std::plus<int>(), 0);
    return global_sum;
}
