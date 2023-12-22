// Copyright 2023 Safarov Nurlan
#include "task_3/safarov_n_int_radix_sort_simple_merge/int_radix_sort_simple_merge.h"

std::vector<int> generateRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> randomVector(size);
    for (int i = 0; i < size; i++) {
        randomVector[i] = gen() % 100;
    }

    return randomVector;
}

void countSortByDigit(int* v, int d, int size) {
    std::vector<int> result(size);
    int count[10] = { 0 };

    for (int i = 0; i < size; i++) {
        count[(v[i] / d) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        result[count[(v[i]/d) % 10] - 1] = v[i];
        count[(v[i]/d) % 10]--;
    }

    for (int i = 0; i < size; i++)
        v[i] = result[i];
}

std::vector<int> radixSortWithSimpleMergeSequential(std::vector<int> vector) {
    const int size = vector.size();
    int max = vector[0];
    for (int i = 0; i < size; i++) {
        max = std::max(max, vector[i]);
    }

    for (int i = 1; max / i > 0; i *= 10) {
        countSortByDigit(vector.data(), i, size);
    }

    return vector;
}

std::vector<int> mergeSortedVectors(std::vector<std::vector<int>> m) {
    if (m.size() == 1) {
        return m[0];
    }
    const int size = m.size();
    std::vector<int> result(m[0].begin(), m[0].end());
    for (int i = 1; i < size; i++) {
        std::vector<int> v = m[i];
        auto iterator = v.begin();
        int k = 0;

        while (k < result.size()) {
            if (iterator != v.end() && result[k] > *iterator) {
                result.insert(result.begin() + k, *iterator);
                iterator++;
            }
            k++;
        }
        while (iterator != v.end()) {
            result.insert(result.end(), *iterator);
            iterator++;
        }
    }
    return result;
}

std::vector<int> radixSortWithSimpleMergeParallel(std::vector<int> vector, int sizeOfVector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = sizeOfVector / size;
    std::vector<int> globalResult(sizeOfVector);

    if (delta != 0) {
        if (rank == 0) {
            for (int processID = 1; processID < size; processID++) {
                MPI_Send(vector.data() + processID*delta, delta, MPI_INT, processID, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<int> localVector(delta);
        if (rank == 0) {
            localVector = std::vector<int>(vector.begin(), vector.begin() + delta);
        } else {
            MPI_Status statusOne;
            MPI_Recv(localVector.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &statusOne);
        }

        std::vector<int> localResult = radixSortWithSimpleMergeSequential(localVector);

        if (rank == 0) {
            std::vector<std::vector<int> > result;
            result.push_back(localResult);
            for (int processID = 1; processID < size; processID++) {
                MPI_Status statusTwo;
                MPI_Recv(localResult.data(), delta, MPI_INT, processID, 0, MPI_COMM_WORLD, &statusTwo);
                result.push_back(localResult);
            }
            globalResult = mergeSortedVectors(result);
        } else {
            MPI_Send(localResult.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    if (sizeOfVector % size != 0 && rank == 0) {
        std::vector<int> localVector = std::vector<int>(vector.end() - sizeOfVector % size, vector.end());
        std::vector<int> localResult = radixSortWithSimpleMergeSequential(localVector);
        std::vector<std::vector<int> > result;
        if (delta != 0) {
            result.push_back(globalResult);
        }
        result.push_back(localResult);
        globalResult = mergeSortedVectors(result);
    }

    return globalResult;
}
