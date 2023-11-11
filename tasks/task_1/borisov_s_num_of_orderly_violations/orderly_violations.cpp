// Copyright 2023 Borisov Saveliy
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/borisov_s_num_of_orderly_violations/orderly_violations.h"

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vector(size);
    for (int  i = 0; i < size; i++) { vector[i] = gen() % 100; }
    return vector;
}

int getViolationsSequentially(std::vector<int> vector) {
    const int  vector_size = vector.size();
    int violations_number = 0;

    for (int index = 1; index < vector_size; index++) {
        if (vector[index] < vector[index - 1]) {
            violations_number++;
        }
    }

    return violations_number;
}

int getViolationsInParallel(std::vector<int> global_vector, int vector_size) {
    boost::mpi::communicator world;

    int process_num = world.size();
    int partial_size = vector_size / process_num;
    int remainder_size = vector_size % process_num;

    std::vector<int> send_counts(process_num, partial_size);
    std::vector<int> displs(world.size(), 0);

    if (world.rank() == 0) {
        for (int i = 1; i < process_num; i++) {
            displs[i] = displs[i - 1] + send_counts[i - 1];
            if (i <= remainder_size) {
                displs[i]++;
            }
        }
    }

    std::vector<int> local_vector(send_counts[world.rank()]);
    printf("\n%d\n", send_counts[world.rank()]);
    boost::mpi::scatterv(world, global_vector, send_counts, displs, local_vector.data(), send_counts[world.rank()], 0);

    int global_violations_number = 0;
    int local_violations_number = getViolationsSequentially(local_vector);
    reduce(world, local_violations_number, global_violations_number, std::plus<int>(), 0);

    return global_violations_number;
}