
// Copyright 2023 Ivanchenko Aleksei
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/ivanchenko_a_gauss_horizontal/gauss_horizontal.h"

std::vector<double>getRandomVector(size_t size, int minElem, int maxElem) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(minElem, maxElem);

    std::vector<double>vec(size);
    for (int  i = 0; i < size; i++) {
        vec[i] = unif(rd);
    }
    return vec;
}
std::vector<double>getRandomMatrix(size_t rows, size_t columns, int minElem, int maxElem) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(minElem, maxElem);
    std::vector<double>m(rows * columns);
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            m[i*columns + j] = unif(rd);

    return m;
}
std::vector<double>gaussSequentional(std::vector<double> A,
 std::vector<double> b) {
    const size_t size = b.size();

    std::vector<double>res(size);
    std::vector<double>c(size, 0);
    // forward
    for (int k = 0; k < size; k++) {
        for (int i = k + 1; i < size; i++) {
            c[i] = A[i*size + k] / A[k * size + k];
        }
        for (int i = k + 1; i < size; i++) {
            for (int j = 0; j < size; j++) {
                A[i*size + j] = A[i*size + j] - c[i] * A[k*size + j];
            }
            b[i] = b[i] - c[i] * b[k];
        }
    }
    // back
    res[size - 1] = b[size - 1] / A[(size - 1) * size + size - 1];
    for (int i = size - 2; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < size; j++) {
            sum = sum + A[i*size + j] * res[j];
        }
        res[i] = (b[i]-sum) / A[i*size + i];
    }

    return res;
}
std::vector<double>gaussParallel(std::vector<double>A,
 std::vector<double>b) {
    boost::mpi::communicator comm;


    const size_t size = b.size();

    if (size < comm.size()) return gaussSequentional(A, b);
    std::vector<double>res(size);
    boost::mpi::broadcast(comm, &A[0], size*size, 0);
    boost::mpi::broadcast(comm, &b[0], size, 0);

    size_t m[size];
    for (int i = 0; i < size; i++) {
        m[i] = i % comm.size();
    }
    // forward
    std::vector<double>c(size);
    for (int k = 0; k < size; k++) {
        boost::mpi::broadcast(comm, &A[k*size + k], size - k, m[k]);
        boost::mpi::broadcast(comm, &b[k], 1, m[k]);
        for (int i = k + 1; i < size; i++) {
            if (m[i] == comm.rank()) {
                c[i] = A[i*size + k] / A[k * size + k];
            }
        }
        for (int i = k + 1; i < size; i++) {
            if (m[i] == comm.rank()) {
                for (int j = 0; j < size; j++) {
                    A[i*size + j] = A[i*size + j] - c[i] * A[k*size + j];
                }
                b[i] = b[i] - c[i] * b[k];
            }
        }
    }
    // back
    if (comm.rank() == 0) {
        res[size-1] = b[size-1] / A[(size-1) * size + size-1];
        for (int i = size - 2; i >= 0; i--) {
            double sum = 0;
            for (int j = i + 1; j < size; j++) {
                sum = sum + A[i*size + j] * res[j];
            }
            res[i] = (b[i]-sum) / A[i*size + i];
        }
    }


    return res;
}
