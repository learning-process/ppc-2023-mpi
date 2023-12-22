// Copyright 2023 Travin Maksim
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/travin_m_yakoby_method/yakoby_method.h"

std::vector<double> get_rand_matrix(int size, int min, int max) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib(min, max);
    std::vector<double> vec(size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            vec[i * size + j] = distrib(gen);
            if ((vec[i * size + j] == 0) && (i == j))
                vec[i * size + j]++;
        }
    }
    return vec;
}

std::vector<double> get_rand_vector(int size, int  min, int max) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib(min, max);
    std::vector<double> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = distrib(gen);
    }
    return vec;
}

std::vector<double> Yakobi(const std::vector<double>& A,
    const std::vector<double>& b, int num_iterations) {
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int n = b.size();

    if (A.size() / n != n)
        throw std::exception();

    for (int i = 0; i < b.size(); i++) {
        if (A[i * n + i] == 0)
            throw std::exception();
    }

    int chunk = b.size() / numProc;
    int remain = b.size() % numProc;

    std::vector<int> procSize(numProc, chunk * n);
    std::vector<int> procDisp(numProc, 0);

    for (int i = 0; i < numProc; i++) {
        if (i < remain) procSize[i] += n;
        procDisp[i] = i == 0 ? 0 : procDisp[i - 1] + procSize[i - 1];
    }

    std::vector<double> x(n, 0.0);
    std::vector<double> new_x(n, 0.0);
    int elem_amount = procSize[rankProc];
    std::vector<double> local_a(elem_amount);

    std::vector<int> vecSize(numProc, chunk);
    std::vector<int> vecDisp(numProc, 0);

    for (int i = 0; i < numProc; i++) {
        if (i < remain) vecSize[i]++;
        vecDisp[i] = i == 0 ? 0 : vecDisp[i - 1] + vecSize[i - 1];
    }
    std::vector<double> local_b(vecSize[rankProc]);

    MPI_Scatterv(A.data(), procSize.data(), procDisp.data(),
            MPI_DOUBLE, local_a.data(), elem_amount,
            MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b.data(), vecSize.data(), vecDisp.data(),
        MPI_DOUBLE, local_b.data(), local_b.size(),
        MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (int k = 0; k < num_iterations; k++) {
        for (int i = 0; i < elem_amount / n; i++) {
            new_x[i] = local_b[i];

            for (int j = 0; j < n; j++) {
                if (j != (vecDisp[rankProc] + i))
                    new_x[i] -= local_a[i * n + j] * x[j];
            }
            new_x[i] /= local_a[i * n + vecDisp[rankProc] + i];
        }

        MPI_Allgatherv(new_x.data(), local_b.size(), MPI_DOUBLE,
            x.data(), vecSize.data(), vecDisp.data(),
            MPI_DOUBLE, MPI_COMM_WORLD);
    }
    return x;
}
