// Copyright 2023 Afanasyev Aleksey
#include "task_1/afanasyev_a_maximum_values_by_matrix_rows/max_values_by_matrix_rows.h"

std::vector<int> CalculateMaxValuesByMatrixRowsSequential(
    const std::vector<int>& matrix,
    size_t rowSize,
    size_t rowCount) {
    std::vector<int> maxValues;

    for (size_t i = 0; i < rowCount; i++) {
        int maxValue = matrix[i * rowSize];

        for (size_t j = 1; j < rowSize; j++) {
            int currentValue = matrix[i * rowSize + j];

            if (currentValue > maxValue) {
                maxValue = currentValue;
            }
        }

        maxValues.push_back(maxValue);
    }

    return maxValues;
}

std::vector<int> CalculateMaxValuesByMatrixRowsParallel(
    const std::vector<int>& matrix,
    size_t rowSize,
    size_t rowCount) {
    if (rowSize == 1 && rowCount == 1)
        return { matrix[0] };
    else if (rowSize <= 0 || rowCount <= 0)
        return { 0 };
    else if (rowSize == 1)
        return matrix;
    else if (rowCount == 1)
        return { *max_element(matrix.begin(), matrix.end()) };

    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int rowsPerProcess = rowCount / worldSize;
    int leftoverRows = rowCount % worldSize;

    int startRow = worldRank * rowsPerProcess + std::min(worldRank, leftoverRows);
    int endRow = startRow + rowsPerProcess + (worldRank < leftoverRows);

    std::vector<int> localMaxValues(endRow - startRow);

    for (int i = startRow; i < endRow; ++i) {
        localMaxValues[i - startRow] = *std::max_element(
            matrix.begin() + i * rowSize,
            matrix.begin() + (i + 1) * rowSize);
    }

    if (worldRank == 0) {
        for (int proc = 1; proc < worldSize; ++proc) {
            startRow = proc * rowsPerProcess + std::min(proc, leftoverRows);
            endRow = startRow + rowsPerProcess + (proc < leftoverRows);
            std::vector<int> procMaxValues(endRow - startRow);
            MPI_Recv(procMaxValues.data(), endRow - startRow, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            localMaxValues.insert(localMaxValues.end(), procMaxValues.begin(), procMaxValues.end());
        }
    } else {
        MPI_Send(localMaxValues.data(), endRow - startRow, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return localMaxValues;
}
