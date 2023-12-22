// Copyright 2023 Vinokurov Ivan
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <cmath>

#include "task_2/vinokurov_i_seidel_iteration_method/seidel_iteration_method.h"

std::vector<double> funcSystemSolveSeidelMPI(std::vector<std::vector<double>>& _mtxA, const std::vector<double>& _vectorB, int _numRows, double _eps)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = _numRows * size;
    int k = 0;

    std::vector<double> x(_numRows, 0.0);
    std::vector<double> xNew(_numRows, 0.0);
    std::vector<double> recvBuff(_numRows, 0.0);

    bool converged = false;
    while (!converged) {
        k++;
        if (k > 100000) {
            // having this many cycle repetitions means there are no roots for this system
            return std::vector<double>(_numRows, 0.0);
        }
        for (int i = 0; i < _numRows; ++i) {
            double sum1 = 0.0, sum2 = 0.0;
            for (int j = 0; j < i + rank * _numRows; ++j) {
                sum1 += _mtxA[i][j] * xNew[j - rank * _numRows];
            }
            for (int j = i + 1 + rank * _numRows; j < n; ++j) {
                sum2 += _mtxA[i][j] * x[j - rank * _numRows];
            }
            xNew[i] = (_vectorB[i] - sum1 - sum2) / _mtxA[i][i + rank * _numRows];
        }

        
        if (rank > 0) {
            MPI_Send(xNew.data(), _numRows, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
        }

       
        if (rank < size - 1) {
            MPI_Recv(xNew.data(), _numRows, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        
        if (rank < size - 1) {
            MPI_Send(xNew.data() + (_numRows - 1), _numRows, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        }

        
        if (rank > 0) {
            MPI_Recv(xNew.data() + (_numRows - 1), _numRows, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        
        if (rank > 0) {
            for (int i = 0; i < _numRows; ++i) {
                xNew[i] = recvBuff[i];
            }
        }

        if (rank < size - 1) {
            for (int i = _numRows; i < 2 * _numRows; ++i) {
                xNew[i - _numRows] = recvBuff[i];
            }
        }

        
        double local_max_diff = 0.0;
        for (int i = 0; i < _numRows; ++i) {
            double diff = std::abs(xNew[i] - x[i]);
            if (diff > local_max_diff) {
                local_max_diff = diff;
            }
        }


        double global_max_diff;
        MPI_Allreduce(&local_max_diff, &global_max_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if (global_max_diff < _eps) {
            converged = true; 
        }

        x = xNew;
    }

    return x; 
}
