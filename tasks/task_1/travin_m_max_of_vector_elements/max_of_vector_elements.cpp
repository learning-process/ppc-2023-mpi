// Copyright 2023 Travin Maksim
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/travin_m_max_of_vector_elements/max_of_vector_elements.h"



std::vector<int> get_random_vector(size_t size, int min_elem, int max_elem) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib(min_elem, max_elem);

    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = distrib(gen);
    }
    return vec;
}

int MaxSeq(const std::vector<int>& vec) {
    int res = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > res)
            res = vec[i];
    }

    return res;
}

int get_max_element(const std::vector<int>& vec) {
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int vecSize = vec.size();

    std::vector<int> recv_count(numProc);
    std::vector<int> displs(numProc);

    int part_size = vecSize / numProc;
    int remain = vecSize % numProc;

    for (int i = 0; i < numProc; i++) {
        recv_count[i] = part_size;
        if (i < remain) recv_count[i]++;
        if (i > 0) {
            displs[i] = displs[i - 1] + recv_count[i - 1];
        } else {
            displs[i] = 0;
            }
    }

    std::vector<int> recv_data(recv_count[rankProc]);
    MPI_Scatterv(vec.data(), recv_count.data(), displs.data(), MPI_INT,
        recv_data.data(), recv_count[rankProc], MPI_INT, 0, MPI_COMM_WORLD);

    int localMax = MaxSeq(recv_data);
    int globalMax = 0;

    MPI_Reduce(&localMax, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return globalMax;
}

