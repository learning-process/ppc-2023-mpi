// Copyright 2023 Kashin Stepa
#include "task_2/kashin_s_gauss_method_horizontal/gauss_method_horizontal.h"
std::vector<double> getResult(double* A, int n) {
    std::vector<double> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = A[i * (n + 1) + n];
    }
    return vec;
}

void gaussianEliminationPAR(double* A, int n, int size, int rank) {
    int recvcount;
    double* subarray;
    std::vector<int> sendCounts(size), displs(size);
    std::vector<double> line(n+1);

    for (int i = 0; i < n - 1; ++i) {
        int colRaws = n - i - 1;

        if (rank == 0) {
            if (size >= colRaws) {
                for (int j = 0; j < colRaws; ++j) {
                    sendCounts[j] = n+1;
                    displs[j] = (n+1) * (i+1+j);
                }
            } else {
                int minCount = colRaws / size;
                int ost = colRaws % size;
                sendCounts[0] = minCount*(n+1);
                displs[0] = (i+1)*(n+1);
                for (int j = 1; j < size; ++j) {
                    sendCounts[j] = minCount*(n+1);
                    displs[j] = sendCounts[j-1]+displs[j-1];
                }
                sendCounts[size - 1] += ost * (n + 1);
            }
            line = std::vector<double>(A + i * (n + 1),A + i * (n + 1)+n+1);

        }
        MPI_Bcast(line.data(), n+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(sendCounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);

        recvcount = sendCounts[rank];
        subarray = new double[recvcount];

        MPI_Scatterv(A, sendCounts.data(), displs.data(), MPI_DOUBLE, subarray, recvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);


        for (int l = 0; l < recvcount; l += n + 1) {
            double ratio = subarray[l + i] / line[i];
            for (int j = i; j < n + 1; ++j) {
                subarray[l + j] -= ratio * line[j];
            }
        }

        MPI_Gatherv(subarray, recvcount, MPI_DOUBLE, A, sendCounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        delete[] subarray;
    }

    if (rank == 0)
    {
        for (int i = n - 1; i >= 0; --i) {
            double sum = 0;
            for (int j = i + 1; j < n; ++j) {
                sum += A[i * (n + 1) + j] * A[j * (n + 1) + n];
            }
            A[i * (n + 1) + n] = (A[i * (n + 1) + n] - sum) / A[i * (n + 1) + i];
        }
    }
}

void generateGaussianMatrix(double* matrix, int n, int max) {

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n + 1; ++j) {
            matrix[i * (n + 1) + j] = std::rand() % max;
        }
    }
}

void gaussianElimination(double* A, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int k = i + 1; k < n; ++k) {
            double ratio = A[k * (n + 1) + i] / A[i * (n + 1) + i];
            for (int j = 0; j < n + 1; ++j) {
                A[k * (n + 1) + j] -= ratio * A[i * (n + 1) + j];
            }
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        double sum = 0;
        for (int j = i + 1; j < n; ++j) {
            sum += A[i * (n + 1) + j] * A[j * (n + 1) + n];
        }
        A[i * (n + 1) + n] = (A[i * (n + 1) + n] - sum) / A[i * (n + 1) + i];
    }

}