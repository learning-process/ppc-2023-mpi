// Copyright 2023 Ivanov Nikita
#include <vector>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/ivanov_gauss_lent_horiz/gauss_lent_horiz.h"
#define ESTIMATE 0.0001

int get_slice(int size, int proc_count, int rank) {
    int q = size / proc_count;
    if (size % proc_count)
        q++;
    int r = proc_count * q - size;

    int slice = q;
    if (rank >= proc_count - r)
        slice = q - 1;
    return slice;
}

std::vector<double> Gauss(std::vector<double> matrix, int size) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int comm_size = world.size();

    std::vector<double> Xi(size);
    int nrows = get_slice(size, comm_size, rank);

    std::vector<int> rows(nrows);
    std::vector<double> tmp(size + 1);

    for (int i = 0; i < nrows; i++)
        rows[i] = rank + comm_size * i;


    int row = 0;
    for (int i = 0; i < size - 1; i++) {
        if (i == rows[row]) {
            boost::mpi::broadcast(world, &matrix.data()[rows[row] * (size + 1)], size + 1, rank);
            for (int j = 0; j <= size; j++)
                tmp[j] = matrix[rows[row] * (size + 1) + j];
            row++;
        } else {
            boost::mpi::broadcast(world, tmp.data(), size + 1, i % comm_size);
        }

        for (int j = row; j < nrows; j++) {
            double scaling = matrix[rows[j] * (size + 1) + i] / tmp[i];
            for (int k = i; k < size + 1; k++)
                matrix[rows[j] * (size + 1) + k] -= scaling * tmp[k];
        }
    }

    row = 0;
    for (int i = 0; i < size; i++) {
        Xi[i] = 0;
        if (i == rows[row]) {
            Xi[i] = matrix[i * (size + 1) + size];
            row++;
        }
    }

    row = nrows - 1;
    for (int i = size - 1; i > 0; i--) {
        if (row >= 0) {
            if (i == rows[row]) {
                Xi[i] /= matrix[i * (size + 1) + i];
                boost::mpi::broadcast(world, &(Xi.data())[i], 1, rank);
                row--;
            } else {
                boost::mpi::broadcast(world, &(Xi.data())[i], 1, i % comm_size);
            }
        } else {
            boost::mpi::broadcast(world, &(Xi.data())[i], 1, i % comm_size);
        }

        for (int j = 0; j <= row; j++)
            Xi[rows[j]] -= matrix[rows[j] * (size + 1) + i] * Xi[i];
    }

    if (rank == 0)
        Xi[0] /= matrix[rows[row] * (size + 1)];

    return Xi;
}

bool check_result(std::vector<double> matrix, std::vector<double> x, int size) {
    for (int i = 0; i < size; i++) {
        double sum = 0;
        for (int j = 0; j < size; j++)
            sum += matrix[i * (size + 1) + j] * x[j];

        if (std::abs(sum - matrix[i * (size + 1) + size]) > ESTIMATE)
            return false;
    }
    return true;
}

std::vector<double> create_random_matrix(int size) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(-100, 100);
    std::vector<double> matrix((size + 1) * size);
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size + 1; j++)
            matrix[i * (size + 1) + j] = unif(rd);

    return matrix;
}
