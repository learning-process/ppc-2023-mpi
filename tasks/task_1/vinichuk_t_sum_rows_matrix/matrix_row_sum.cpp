// Copyright 2023 Vinichuk Timofey
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include "task_1/vinichuk_t_sum_rows_matrix/matrix_row_sum.h"


std::vector<int> RandomVector(int  w, int h) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(w*h);
    for (int i = 0; i < w*h; i++) { matrix[i] = gen() % 100; }
    return matrix;
}

std::vector<int> SeqSumRowsMatrix(const std::vector<int>& matrix, int w) {
    std::vector<int> v(matrix.size()/w, 0);
    for (int y = 0; y < matrix.size()/w; y++) {
        for (int x = 0; x < w; x++) {
            v[y] += matrix[y*w + x];
        }
    }
    return v;
}

std::vector<int> ParallelSumRowsMatrix(const std::vector<int>& matrixt, int w) {
    boost::mpi::communicator world;
    std::vector<int> matrix(matrixt);
    boost::mpi::broadcast(world, matrix, 0);
    int start;
    int delta = matrix.size() / world.size();
    if (world.rank() < matrix.size() % world.size()) {
        delta += 1;
        start = world.rank() * delta;
    } else {
        start = (matrix.size() % world.size()) * (delta + 1) +
            delta * (world.rank() - (matrix.size() % world.size()));
    }
    // printf("delta=%d, start=%d", delta, start);
    std::vector<int> v(matrix.size() / w, 0);
    for (int i = start; i < start + delta; i++) {
        v[i / w] += matrix[i ];
    }
    std::vector<int> gathered_data(matrix.size() / w, 0);
    boost::mpi::reduce(world, v, gathered_data, std::plus<int>(), 0);

    return gathered_data;
}
