// Copyright Zhatkin Vyacheslav

#include "task_1/zhatkin_v_scalar_producr/scalar_product.h"


std::vector<int> creatRandomVector(const int v_size) {
    std::vector<int> vector(v_size);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < v_size; i++) {
        vector[i] = gen() % MAX_NUMBER + MIN_NUMBER;
    }
    return vector;
}

int getSequentialScalarProduct(const std::vector<int>& firsVector,
    const std::vector<int>& secondVector) {
    if (firsVector.size() != secondVector.size()) {
        throw(1);
    }
    int result = 0;
    for (unsigned int i = 0; i < firsVector.size(); i++) {
        result += firsVector[i] * secondVector[i];
    }
    return result;
}

int getParallelScalarProduct(const std::vector<int>& firsVector,
    const std::vector<int>& secondVector) {
    int rank, size;
    if (firsVector.size() != secondVector.size()) {
        throw(1);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int v_size = firsVector.size() / size;
    int buffer = firsVector.size() % size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int result = 0, result_other = 0;

    if (rank == 0) {
        for (int proc = 1;
            proc < size; ++proc) {
            if (proc == size - 1) {
                MPI_Send(&firsVector[0] + proc * v_size, v_size + buffer,
                    MPI_INT, proc, 2, MPI_COMM_WORLD);
                MPI_Send(&secondVector[0] + proc * v_size, v_size + buffer,
                    MPI_INT, proc, 3, MPI_COMM_WORLD);
            } else {
                MPI_Send(&firsVector[0] + proc * v_size, v_size,
                    MPI_INT, proc, 0, MPI_COMM_WORLD);
                MPI_Send(&secondVector[0] + proc * v_size, v_size,
                    MPI_INT, proc, 1, MPI_COMM_WORLD);
            }
        }
    }

    std::vector<int> locA(v_size);
    std::vector<int> locB(v_size);


    if (rank == 0) {
        locA = std::vector<int>(firsVector.begin(),
            firsVector.begin() + v_size);
        locB = std::vector<int>(secondVector.begin(),
            secondVector.begin() + v_size);
    } else if (rank == size - 1) {
        locA.resize(v_size + buffer);
        locB.resize(v_size + buffer);

        MPI_Status status;
        MPI_Recv(&locA[0], v_size + buffer, MPI_INT, 0, 2,
            MPI_COMM_WORLD, &status);
        MPI_Recv(&locB[0], v_size + buffer, MPI_INT, 0, 3,
            MPI_COMM_WORLD, &status);
    } else {
        MPI_Status status;
        MPI_Recv(&locA[0], v_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&locB[0], v_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    if (rank == size - 1) {
        for (int i = 0; i < v_size + buffer; ++i) {
            result_other += locA[i] * locB[i];
        }
    } else {
        for (int i = 0; i < v_size; ++i) {
            result_other += locA[i] * locB[i];
        }
    }

    MPI_Reduce(&result_other, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return result;
}
