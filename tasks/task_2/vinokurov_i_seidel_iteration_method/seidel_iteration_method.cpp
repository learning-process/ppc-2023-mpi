// Copyright 2023 Vinokurov Ivan
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <cmath>
#include "task_2/vinokurov_i_seidel_iteration_method/seidel_iteration_method.h"

std::vector<double> funcSystemSolveSeidelMPI(const std::vector<std::vector<double>>& _mtxA,
                                             const std::vector<double>& _vectorB,
                                             int _numRows, double _eps) {
    int size, _rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int k = 0;

    int blockSize = _numRows / size;
    int remainingRows = _numRows % size;

    int startRow = _rank * blockSize + std::min(_rank, remainingRows);
    int endRow = startRow + blockSize + (_rank < remainingRows ? 1 : 0);

    int localRowCount = endRow - startRow;

    int totalRowCount;
    MPI_Allreduce(&localRowCount, &totalRowCount, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    std::vector<double> x(_numRows, 0.0);
    std::vector<double> xNew(_numRows, 0.0);

    bool converged = false;
    while (!converged) {
        k++;
        if (k > 100) {
            // having this many cycle repetitions means there are no roots for this system
            return std::vector<double>(_numRows, 0.0);
        }
        for (int i = startRow; i < endRow; ++i) {
            double sum1 = 0.0, sum2 = 0.0;
            for (int j = 0; j < i; ++j) {
                sum1 += _mtxA[i][j] * xNew[j];
            }
            for (int j = i + 1; j < _numRows; ++j) {
                sum2 += _mtxA[i][j] * x[j];
            }
            xNew[i] = (_vectorB[i] - sum1 - sum2) / _mtxA[i][i];
        }

        MPI_Allgather(&xNew[startRow], localRowCount, MPI_DOUBLE,
            &xNew[0], localRowCount, MPI_DOUBLE, MPI_COMM_WORLD);

        double localMaxDiff = 0.0;
        for (int i = 0; i < _numRows; ++i) {
            double diff = std::abs(xNew[i] - x[i]);
            if (diff > localMaxDiff) {
                localMaxDiff = diff;
            }
        }

        double globalMaxDiff;
        MPI_Allreduce(&localMaxDiff, &globalMaxDiff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if (globalMaxDiff < _eps) {
            converged = true;
        }

        x = xNew;
    }
    return x;
}
