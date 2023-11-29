// Copyright 2023 Ivanchenko Aleksei
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "./max_across_columns.h"

std::vector<int> getRandomMatrix(int rows, int columns) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(rows*columns);
    for (int  i = 0; i < vec.size(); i++) {
         vec[i] = gen() % 100; 
    }
    return vec;
}

int getSequentialOperations(std::vector<int> vec, int rows, int columns) {
    // returns vector of max values across columns
    // sequential algorithm
    std::vector<int> result(columns, INT_MIN);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns) {
            result[j] = std::max(result[j], vec[i*columns + j]);
        }
    }
    return result;
}

int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector, const std::string& ops) {
    boost::mpi::communicator world;
    const int delta = count_size_vector / world.size();

    if (world.rank() == 0) {
        for (int proc = 1; proc < world.size(); proc++) {
            world.send(proc, 0, global_vec.data() + proc * delta, delta);
        }
    }

    std::vector<int> local_vec(delta);
    if (world.rank() == 0) {
        local_vec = std::vector<int>(global_vec.begin(),
                                     global_vec.begin() + delta);
    } else {
        world.recv(0, 0, local_vec.data(), delta);
    }

    int global_sum = 0;
    int local_sum = getSequentialOperations(local_vec, ops);
    if (ops == "+" || ops == "-") {
        reduce(world, local_sum, global_sum, std::plus<int>(), 0);
    }
    if (ops == "max") {
        reduce(world, local_sum, global_sum, boost::mpi::maximum<int>(), 0);
    }
    return global_sum;
}
