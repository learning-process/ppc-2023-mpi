
// Copyright 2023 Ivanchenko Aleksei
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/ivanchenko_a_gauss_horizontal/gauss_horizontal.h"

std::vector<int> getMaxParallel(const std::vector<int>& matrix, size_t rows, size_t columns) {
    boost::mpi::communicator comm;
    std::vector<int> res(columns);
    size_t t1 = 0;
    std::vector<int> localSizes(comm.size(), t1);
    std::vector<int> localMatrix(t1);
    if (t1 == 0) {
    } else {
        boost::mpi::scatterv(comm, localMatrix.data(), localSizes[comm.rank()], 0);
    }
    return res;
}

boost::numeric::ublas::vector<double>getRandomVector(size_t size, int minElem, int maxElem) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(minElem, maxElem);

    boost::numeric::ublas::vector<double>vec(size);
    for (int  i = 0; i < size; i++) {
        vec(i) = unif(rd);
    }
    return vec;
}
boost::numeric::ublas::matrix<double>getRandomMatrix(size_t rows, size_t columns, int minElem, int maxElem) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(minElem, maxElem);
    boost::numeric::ublas::matrix<double>m(rows, columns);
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            m(i, j) = unif(rd);

    return m;
}
boost::numeric::ublas::vector<double>gaussSequentional(boost::numeric::ublas::matrix<double>A, boost::numeric::ublas::vector<double>b) {
    // Solve the system of equations
    boost::numeric::ublas::permutation_matrix<std::size_t> pm(A.size1());
    lu_factorize(A, pm);
    lu_substitute(A, pm, b);
    return b;
}
boost::numeric::ublas::vector<double>gaussParallel(boost::numeric::ublas::matrix<double>A, boost::numeric::ublas::vector<double>b) {
    boost::mpi::communicator comm;


    size_t rows = A.size1(), columns = A.size2();
    // assert rows == columns
    const size_t N = rows;

    if (N < comm.size()) return gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>res(N);
    boost::mpi::broadcast(comm, A, 0);
    boost::mpi::broadcast(comm, b, 0);

    size_t m[N];
    for (int i = 0; i < N; i++) {
        m[i] = i % comm.size();
    }
    boost::numeric::ublas::vector<double>c(N);
    for (int k = 0; k < N; k++) {
        boost::mpi::broadcast(comm, &A(k, k), N - k, m[k]);
        boost::mpi::broadcast(comm, &b[k], 1, m[k]);
        for (int i = k + 1; i < N; i++) {
            if (m[i] == comm.rank()) {
                c(i) = A(i, k) / A(k, k);
            }
        }
        for (int i = k + 1; i < N; i++) {
            if (m[i] == comm.rank()) {
                for (int j = 0; j < N; j++) {
                    A(i, j) = A(i, j) - c(i) * A(k, j);
                }
                b(i) = b(i) - c(i) * b(k);
            }
        }
    }
    // back
    if (comm.rank() == 0) {
        res(N-1) = b(N-1) / A(N-1, N-1);
        for (int i = N - 2; i >= 0; i--) {
            double sum=0;
            for (int j = i + 1; j < N; j++) {
                sum = sum + A(i, j) * res(j);
            }
            res(i) = (b(i)-sum) / A(i, i);
        }
    }


    return res;
}
