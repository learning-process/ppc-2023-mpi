// Copyright 2023 Kulikov Artem
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>

#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/vector.hpp>

#include "task_2/kulikov_a_strip_Mmul_hA_vB/strip_Mmul_hA_vB.h"

std::vector<int> getRandomMatrix(int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matr(n * n);
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            matr[i * n + j] = gen() % 100;
        }
    }
    return matr;
}

std::vector<int> getParallelMatrMul(std::vector<int> A, std::vector<int> B, int n) {
    boost::mpi::communicator world;
    std::vector<int> C(n * n);
    int rank = world.rank(), p = world.size(), c_i;
    int prev = (p + rank - 1) % p, next = (rank + 1) % p, delta = n / p;
    std::vector<int> Arow(n * delta), Bcol(n * delta), Crow(n * delta);

    boost::mpi::scatter(world, A, Arow.data(), n * delta, 0);
    boost::mpi::scatter(world, B, Bcol.data(), n * delta, 0);

    if (world.rank() == 0) {
        Arow = std::vector<int>(A.begin(), A.begin() + n * delta);
        Bcol = std::vector<int>(B.begin(), B.begin() + n * delta);
    }

    for (int dx = 0; dx < delta; dx++) {
        for (int dy = 0; dy < delta; dy++) {
            c_i = (p + rank - 0) % p;
            c_i = c_i * delta + dx + dy * n;
            Crow[c_i] = 0;
            for (int j = 0; j < n; j++) Crow[c_i] += Arow[j + dy * n] * Bcol[j + dx * n];
        }
    }

    for (int i = 1; i < p; i++) {
        world.send(next, rank + p * i, Bcol);
        world.recv(prev, prev + p * i, Bcol);

        for (int dx = 0; dx < delta; dx++) {
            for (int dy = 0; dy < delta; dy++) {
                c_i = (p + rank - i) % p;
                c_i = c_i * delta + dx + dy * n;
                Crow[c_i] = 0;
                for (int j = 0; j < n; j++) Crow[c_i] += Arow[j + dy * n] * Bcol[j + dx * n];
            }
        }
    }
    boost::mpi::gather(world, Crow.data(), n * delta, C.data(), 0);
    return C;
}

std::vector<int> getSequentialMatrMul(std::vector<int> A, std::vector<int> B, int n) {
    std::vector<int> C(n * n);
    int i, j, k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (k = 0; k < n; k++) {
                C[i * n + j] = C[i * n + j] + A[i * n + k] * B[j * n + k];
            }
        }
    }
    return C;
}
