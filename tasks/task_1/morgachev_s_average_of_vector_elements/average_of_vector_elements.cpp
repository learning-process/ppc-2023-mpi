// Copyright 2023 Morgachev Stepan
#include "task_1/morgachev_s_average_of_vector_elements/average_of_vector_elements.h"

std::vector<int> fillVectorRandomNumbers(size_t size, int min, int max) {
    std::vector<int> vector(size);

    for (auto& element : vector) {
        element = min + (std::rand() % (max - min + 1));
    }

    return vector;
}

int calculatePartialSum(const std::vector<int>& vec) {
    int sum = 0;

    for (int i : vec) {
        sum += i;
    }

    return sum;
}

double calculateAverageOfVectorElements(const std::vector<int>& vector) {
    int sizeWorld = 0;
    int rank = 0;
    size_t vectorSize = vector.size();

    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunkSize = vectorSize / sizeWorld;
    int remainder = vectorSize % sizeWorld;

    std::vector<int> sendCounts(sizeWorld);
    std::vector<int> displacement(sizeWorld);

    int dif = 0;

    for (int i = 0; i < sizeWorld; i++) {
        if (i < remainder) {
            sendCounts[i] = chunkSize + 1;
        } else {
            sendCounts[i] = chunkSize;
        }
        displacement[i] = dif;
        dif += sendCounts[i];
    }

    std::vector<int> localVector(sendCounts[rank]);

    MPI_Scatterv(vector.data(), sendCounts.data(), displacement.data(), MPI_INT,
        localVector.data(), sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int totalSum = 0;
    int partialSum = calculatePartialSum(localVector);

    MPI_Reduce(&partialSum, &totalSum, 1, MPI_INT, MPI_SUM, 0,
        MPI_COMM_WORLD);

    return totalSum / static_cast<double>(vectorSize);
}
