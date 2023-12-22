// Copyright 2023 Veselov Mihail
#include <random>
#include <iostream>
#include "task_1/veselov_m_sum_of_vector_elements/sum_of_vector_elements.h"

int sum_of_vec_elem_seq(const std::vector<int> &vec) {
    int res = 0;
    for (int i = 0; i < vec.size(); i++) {
        res += vec[i];
    }

    return res;
}

int sum_of_vec_elem_par(const std::vector<int> &vec) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);


    std::vector<int> recv_counts(ProcNum);
    std::vector<int> recv_displs(ProcNum);

    const int chunk = vec.size() / ProcNum;
    const int tail = vec.size() % ProcNum;

      for (int i = 0; i < ProcNum; i++) {
        recv_counts[i] = chunk;
        if (i < tail) recv_counts[i]++;

        recv_displs[i] = (i > 0) ? (recv_displs[i - 1] + recv_counts[i - 1]) : 0;
      }

    std::vector<int> recv_vec(recv_counts[ProcRank]);
    MPI_Scatterv(vec.data(), recv_counts.data(), recv_displs.data(), MPI_INT,
                recv_vec.data(), recv_counts[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);

    int res = 0;
    int sum = sum_of_vec_elem_seq(recv_vec);

    MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return res;
}

void print_vec(const std::vector<int> &vec) {
    for (int i = 0; i < vec.size(); i++) {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

std::vector<int> get_random_vector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}
