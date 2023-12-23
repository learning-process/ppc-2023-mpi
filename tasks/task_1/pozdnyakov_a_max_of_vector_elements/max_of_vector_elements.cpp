// Copyright 2023 Pozdnyakov Vasya
#include "task_1/pozdnyakov_a_max_of_vector_elements/max_of_vector_elements.h"

std::vector<int> get_random_vector(size_t size, int min_elem, int max_elem) {
    std::vector<int> vec(size);
    for (size_t  i = 0; i < size; i++) {
        vec[i] = min_elem + (std::rand() % (max_elem - min_elem + 1));
    }
    return vec;
}

int get_max_element(const std::vector<int>& init_vec, size_t vec_size) {
     boost::mpi::communicator world;
    const int local_vec_size = vec_size / world.size();
    const int remain = vec_size % world.size();
    std::vector<int> local_vec(local_vec_size);

    if (world.rank() == 0) {
        if (local_vec_size != 0) {
            for (int proc = 1; proc < world.size(); proc++) {
                world.send(proc, 0, init_vec.data() + remain + local_vec_size * proc, local_vec_size);
            }
        } else {
            local_vec = init_vec;
        }
    }
    if (local_vec_size != 0) {
        if (world.rank() == 0) {
            local_vec = std::vector<int>(init_vec.begin(), init_vec.begin() + local_vec_size + remain);
        } else {
            world.recv(0, 0, local_vec.data(), local_vec_size);
        }
    }

    auto local_res = std::max_element(local_vec.begin(), local_vec.end());
    int global_res = 0;
    if (local_vec_size != 0) {
        boost::mpi::reduce(world, *local_res, global_res, boost::mpi::maximum<int>(), 0);
    } else {
        if (world.rank() == 0) {
            global_res = *local_res;
        }
    }

    return global_res;
}
