// Copyright 2023 Pushkarev Ilya

#include "task_2/pushkarev_i_jacobi_method/jacobi_method.h"
#include <random>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::vector<double> Jacobi(const std::vector<double>& coefficients,
                                 const std::vector<double>& right_hand_side,
                                 int num_iterations) {
    int num_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int rank_proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);

    int system_size = right_hand_side.size();

    if (coefficients.size() / system_size != system_size)
        throw std::exception();

    for (int i = 0; i < system_size; i++) {
        if (coefficients[i * system_size + i] == 0)
            throw std::exception();
    }

    int chunk_size = system_size / num_procs;
    int remainder = system_size % num_procs;

    std::vector<int> proc_sizes(num_procs, chunk_size * system_size);
    std::vector<int> proc_displacements(num_procs, 0);

    for (int i = 0; i < num_procs; i++) {
        if (i < remainder) proc_sizes[i] += system_size;
        proc_displacements[i] = (i == 0) ? 0 : proc_displacements[i - 1] + proc_sizes[i - 1];
    }

    std::vector<double> solution(system_size, 0.0);
    std::vector<double> new_solution(system_size, 0.0);
    int local_size = proc_sizes[rank_proc];
    std::vector<double> local_coefficients(local_size);

    std::vector<int> vec_sizes(num_procs, chunk_size);
    std::vector<int> vec_displacements(num_procs, 0);

    for (int i = 0; i < num_procs; i++) {
        if (i < remainder) vec_sizes[i]++;
        vec_displacements[i] = (i == 0) ? 0 : vec_displacements[i - 1] + vec_sizes[i - 1];
    }

    std::vector<double> local_rhs(vec_sizes[rank_proc]);

    MPI_Scatterv(coefficients.data(), proc_sizes.data(), proc_displacements.data(),
                 MPI_DOUBLE, local_coefficients.data(), local_size,
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(right_hand_side.data(), vec_sizes.data(), vec_displacements.data(),
                 MPI_DOUBLE, local_rhs.data(), local_rhs.size(),
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int iteration = 0; iteration < num_iterations; iteration++) {
        for (int i = 0; i < local_size / system_size; i++) {
            new_solution[i] = local_rhs[i];

            for (int j = 0; j < system_size; j++) {
                if (j != (vec_displacements[rank_proc] + i))
                    new_solution[i] -= local_coefficients[i * system_size + j] * solution[j];
            }

            new_solution[i] /= local_coefficients[i * system_size + vec_displacements[rank_proc] + i];
        }

        MPI_Allgatherv(new_solution.data(), local_rhs.size(), MPI_DOUBLE,
                      solution.data(), vec_sizes.data(), vec_displacements.data(),
                      MPI_DOUBLE, MPI_COMM_WORLD);
    }

    return solution;
}

std::vector<double> create_random_vector(size_t size, double lower_bound, double upper_bound) {
    std::vector<double> random_vector(size);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);

    for (double &element : random_vector) {
        element = distribution(gen);
    }

    return random_vector;
}

std::vector<double> create_random_matrix(size_t size, double lower_bound, double upper_bound) {
    std::vector<double> random_vector(size*size);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);

    for (double &element : random_vector) {
        element = distribution(gen);
    }

    for (int i = 0; i < size; i++) {
        while (random_vector[i*size + i] == 0) {
            random_vector[i*size + i] = distribution(gen);
        }
    }

    return random_vector;
}
