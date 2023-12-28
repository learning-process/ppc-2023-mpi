// Copyright 2023 Kochetov Nikolay
#include "task_2/kochetov_n_multiplicate_matrix/multiplicate_matrix.h"

std::vector<int> MultiplicateMatrix(
    std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int max_size, int count) {
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < max_size; j++) {
            (*C)[i * max_size + j] = 0;
            for (int k = 0; k < max_size; k++) {
                (*C)[i * max_size + j] +=
                    (*A)[i * max_size + k] * (*B)[k * max_size + j];
            }
        }
    }
    return (*C);
}

std::vector<int> MultiplicateMatrixP(
    std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int max_size) {
    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int portion = max_size / size;
    int remainder = max_size % size;
    std::vector<int> count, sendcount, displ;
    count.resize(size);
    sendcount.resize(size);
    displ.resize(size);
    for (int i = 0; i < size; i++) {
        count[i] = portion;
        if (i < remainder) {
        count[i]++;
        }
    }
    for (int i = 0; i < size; i++) {
        int tmp = count[i] * max_size;
        sendcount[i] = tmp;
    }

    int _displ = 0;
    for (int i = 0; i < size; i++) {
        displ[i] = _displ;
        _displ += sendcount[i];
    }

    std::vector<int> localA(sendcount[rank]);

    MPI_Scatterv((*A).data(), sendcount.data(), displ.data(), MPI_INT,
        localA.data(), sendcount[rank], MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast((*B).data(), max_size * max_size, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> localC(sendcount[rank]);
    localC = MultiplicateMatrix(
        &localA, &(*B), &localC, max_size, count[rank]);

    MPI_Gatherv(localC.data(), sendcount[rank], MPI_INT, (*C).data(),
        sendcount.data(), displ.data(), MPI_INT, 0, MPI_COMM_WORLD);
    return (*C);
}
