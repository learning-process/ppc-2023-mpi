// Copyright 2023 Kruglov Alexey
#include "task_1/kruglov_a_number_of_orderly_violations/number_of_orderly_violations.h"


int numberOfOrderValids_Sync(const std::vector<int>& Vector) {
    int numOfValids = 0;
    int length = Vector.size();
    for (int i = 0; i < length - 1; ++i)
        if (Vector[i] > Vector[i + 1]) {
            numOfValids++;
        }
    return numOfValids;
}

int numberOfOrderValids_ASync(const std::vector<int>& Vector) {
    int numOfValids = 0;
    int length = Vector.size();
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = length / size;
    if (length < size) {
        return numberOfOrderValids_Sync(Vector);
    }
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&delta, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&Vector[0] + (proc - 1) * delta, delta + 1, MPI_INT, proc, 1, MPI_COMM_WORLD);
        }
    }
    std::vector<int> local_vec;
    int size_local_vector = delta + 1;
    if (rank == 0) {
        local_vec = std::vector<int>(Vector.begin() + (size - 1) * delta, Vector.end());
    } else {
        MPI_Status status;
        MPI_Recv(&size_local_vector, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        size_local_vector++;
        local_vec.resize(size_local_vector);
        MPI_Recv(&local_vec[0], size_local_vector, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    int local_num = numberOfOrderValids_Sync(local_vec);
    MPI_Reduce(&local_num, &numOfValids, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return numOfValids;
}

std::vector<int> generateRandomVector(const int& length) {
    std::vector<int> vec(length);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < length; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}
