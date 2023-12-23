// Copyright 2023 Zawadowski Jan
#include "task_1/zawadowski_j_max_matrix_columns/max_matrix_columns.h"

std::vector<int> fillMatrixRandom(size_t rows, size_t cols, int min, int max) {
    std::vector<int> matrix(rows * cols);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = dist(gen);
        }
    }

    return matrix;
}

std::vector<int> findMaxByMatrixColumns(std::vector<int> matrix,
                                        size_t rows,
                                        size_t cols) {
    std::vector<int> maxValues,
                     sendCounts,
                     displacement,
                     vec;
    int sizeWorld = 0,
        rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk = cols / sizeWorld,
        remainder = cols % sizeWorld;
    std::vector<int> localVector(chunk * rows);

    if (rank == 0) {
        maxValues.resize(cols, std::numeric_limits<int>::min());
        sendCounts.resize(sizeWorld, chunk);
        sendCounts[0] = sendCounts[0] + remainder;
        displacement.resize(sizeWorld, 0);
        for (int i = 1; i < sizeWorld; i++)
            displacement[i] = displacement[i - 1] + sendCounts[i - 1];
        localVector.resize((chunk + remainder) * rows);
    }

    MPI_Datatype datatype,
                 column;

    MPI_Type_vector(rows, 1, cols, MPI_INT, &datatype);
    MPI_Type_commit(&datatype);
    MPI_Type_create_resized(datatype, 0, sizeof(int) * 1, &column);
    MPI_Type_commit(&column);

    MPI_Scatterv(matrix.data(),
                 sendCounts.data(),
                 displacement.data(),
                 column,
                 localVector.data(),
                 static_cast<int>(localVector.size()),
                 MPI_INT,
                 0,
                 MPI_COMM_WORLD);

    int localSize = localVector.size() / rows;
    for (int i = 0; i < localSize; i++) {
        int max = std::numeric_limits<int>::min();
        for (int j = 0; j < rows; j++)
            max = std::max(max, localVector[i * rows + j]);
        vec.push_back(max);
    }

    MPI_Gatherv(vec.data(),
                vec.size(),
                MPI_INT,
                maxValues.data(),
                sendCounts.data(),
                displacement.data(),
                MPI_INT,
                0,
                MPI_COMM_WORLD);

    return maxValues;
}

std::vector<int> findMaxByMatrixColumnsConsistently(std::vector<int> matrix,
                                                    size_t rows,
                                                    size_t cols) {
    std::vector<int> maxValues;

    for (int i = 0; i < cols; i++) {
        int max = std::numeric_limits<int>::min();
        for (int j = 0; j < rows; j++)
            max = std::max(max, matrix[j * cols + i]);
        maxValues.push_back(max);
    }

    return maxValues;
}
