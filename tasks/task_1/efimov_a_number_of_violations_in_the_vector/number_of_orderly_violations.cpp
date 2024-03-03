// Copyright 2024 Efimov Andrey
#include "task_1/efimov_a_number_of_violations_in_the_vector/number_of_orderly_violations.h"

std::vector<int> make_random_vector(const int& length) {
    std::vector<int> vec(length);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < length; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

int numberOfOrderValids_ASync(const std::vector<int>& Vector, int Vector_len) {
    int numOfValids = 0;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = Vector_len / size;
    if (Vector_len < size) {
        return numberOfOrderValids_Sync(Vector);
    }
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&Vector[0] + (proc - 1) * delta, delta + 1,
            MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<int> local_vec;
    int size_local_vector = delta + 1;
    if (rank == 0) {
        local_vec = std::vector<int>(Vector.begin() +
        (size - 1) * delta, Vector.end());
    } else {
        MPI_Status status;
        local_vec.resize(size_local_vector);
        MPI_Recv(&local_vec[0], size_local_vector, MPI_INT,
        0, 0, MPI_COMM_WORLD, &status);
    }
    int local_num = numberOfOrderValids_Sync(local_vec);
    MPI_Reduce(&local_num, &numOfValids, 1,
    MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return numOfValids;
}

int numberOfOrderValids_Sync(const std::vector<int>& Vector) {
    int numOfValids = 0;
    int length = Vector.size();
    for (int i = 0; i < length - 1; ++i)
        if (Vector[i] > Vector[i + 1]) {
            numOfValids++;
        }
    return numOfValids;
}
