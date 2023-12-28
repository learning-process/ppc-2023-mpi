#include "task_1/kachalov_m_num_of_orderly_violations/orderly_violations.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <random>
#include <ctime>

int num_of_orderly_violations(const std::vector<int>& vec) {
    int violations = 0;
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            violations++;
        }
    }
    return violations;
}

int parallel_num_of_orderly_violations(const std::vector<int>& vec) {
    int violations = 0;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = vec.size() / size;

    if (vec.size() < size) {
        return num_of_orderly_violations(vec);
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&vec[0] + (proc - 1) * delta, delta + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec;
    int size_local_vector = delta + 1;

    if (rank == 0) {
        local_vec = std::vector<int>(vec.begin() + (size - 1) * delta, vec.end());
    } else {
        MPI_Status status;
        local_vec.resize(size_local_vector);
        MPI_Recv(&local_vec[0], size_local_vector, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int local_num = num_of_orderly_violations(local_vec);
    MPI_Reduce(&local_num, &violations, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return violations;
}

std::vector<int> get_random_vector(const int& length) {
    std::vector<int> vec(length);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < length; i++) {
        vec[i] = gen() % 100;
    }

    return vec;
}
