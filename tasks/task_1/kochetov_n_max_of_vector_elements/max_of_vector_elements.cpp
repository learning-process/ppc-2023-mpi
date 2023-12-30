// Copyright 2023 Kochetov Nikolay
#include "task_1/kochetov_n_max_of_vector_elements/max_of_vector_elements.h"

std::vector<int> GetRandomVector(size_t size, int min_elem, int max_elem) {
    std::vector<int> vec(size);
    for (size_t  i = 0; i < size; i++) {
        vec[i] = min_elem + (std::rand() % (max_elem - min_elem + 1));
    }
    return vec;
}

int GetMaxElement(const std::vector<int>& init_vec, size_t vec_size) {
    boost::mpi::communicator comm;
    const int partSize = vec_size / comm.size();
    std::vector<int> local_vec(partSize);
    const int remain = vec_size % comm.size();

    if (comm.rank() == 0) {
        if (partSize != 0) {
            for (int proc = 1; proc < comm.size(); proc++) {
                comm.send(proc, 0, init_vec.data() + remain +
                    partSize * proc, partSize);
            }
        } else {
            local_vec = init_vec;
        }
    }

    if (partSize != 0) {
        if (comm.rank() == 0) {
            local_vec = std::vector<int>(init_vec.begin(),
                init_vec.begin() + partSize + remain);
        } else {
            comm.recv(0, 0, local_vec.data(), partSize);
        }
    }

    auto temp = std::max_element(local_vec.begin(), local_vec.end());
    int result = 0;
    if (partSize != 0) {
        boost::mpi::reduce(comm, *temp,
            result, boost::mpi::maximum<int>(), 0);
    } else {
        if (comm.rank() == 0) {
            result = *temp;
        }
    }

    return result;
}
