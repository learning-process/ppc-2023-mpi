// Copyright 2023 Khramov Ivan
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include "task_1/khramov_i_vector_dot_product/vector_dot_product.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

std::vector<int> getLocalVectorSizes(int comm_size, int global_vec_size) {
    std::vector<int> vectors_sizes(comm_size, global_vec_size / comm_size);
    vectors_sizes[0] += global_vec_size % comm_size;
    return vectors_sizes;
}

int getSequentialVectorDotProduct(
    const std::vector<int>& vec1,
    const std::vector<int>& vec2) {
    return std::inner_product(
        std::begin(vec1),
        std::end(vec1),
        std::begin(vec2),
        0);
}

int getParallelVectorDotProduct(
    boost::mpi::communicator world,
    int count_size_vector,
    const std::vector<int>& global_vec1,
    const std::vector<int>& global_vec2) {
    int isValid = 1;
    if (world.rank() == 0 &&
     (global_vec1.size() != global_vec2.size()))
        isValid = -1;
    else if (world.rank() == 0 &&
     (global_vec1.size() == 0 || global_vec2.size() == 0))
        isValid = 0;

    boost::mpi::broadcast(world, isValid, 0);

    if (isValid == -1 || isValid == 0)
        return isValid;

    std::vector<int> local_vec1, local_vec2;

    std::vector<int> vectors_sizes = getLocalVectorSizes(
        world.size(),
        count_size_vector);

    local_vec1.resize(vectors_sizes[world.rank()]);
    local_vec2.resize(vectors_sizes[world.rank()]);

    if (world.rank() == 0) {
        boost::mpi::scatterv(
        world,
        global_vec1,
        vectors_sizes,
        local_vec1.data(),
        0);
        boost::mpi::scatterv(
        world,
        global_vec2,
        vectors_sizes,
        local_vec2.data(),
        0);
    } else {
        boost::mpi::scatterv(
        world,
        local_vec1.data(),
        vectors_sizes[world.rank()],
        0);
        boost::mpi::scatterv(
        world,
        local_vec2.data(),
        vectors_sizes[world.rank()],
        0);
    }

    int part_product = getSequentialVectorDotProduct(
        local_vec1,
        local_vec2);

    int global_product;
    boost::mpi::reduce(
        world,
        part_product,
        global_product,
        std::plus<int>(),
        0);

    return global_product;
}
