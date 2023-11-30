// Copyright 2023 Suren Simonyan

#include <utility>
#include "task_3/simonyan_s_quicksort/quicksort.h"

void quickSortFunction(double* array, int left, int right) {
    if (left < right) {
        int i = left;
        int j = right;
        double pivot = array[(left + right) / 2];
        while (i <= j) {
            while (array[i] < pivot)
                i++;
            while (array[j] > pivot)
                j--;
            if (i <= j) {
                std::swap(array[i], array[j]);
                i++;
                j--;
            }
        }
        quickSortFunction(array, left, j);
        quickSortFunction(array, i, right);
    }
}

void Sort(double* arr, int n) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int size = n / ProcNum;
    if (ProcRank == 0) {
        while (ProcNum > 1) {
            int ost = n % ProcNum;
            int offset = 0;
            int zerosize;
            if (ost > 0) {
                zerosize = size + 1;
                ost--;
                offset++;
            } else {
                zerosize = size;
            }
            quickSortFunction(arr, 0, zerosize - 1);
            for (int i = 1; i < ProcNum; ++i) {
                if (ost > 0) {
                    ost--;
                    MPI_Send(&arr[size * i + offset], size + 1,
                        MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                    offset++;
                } else {
                    MPI_Send(&arr[size * i + offset], size,
                        MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                }
            }
            ost = n % ProcNum;
            if (ost > 0) {
                offset = 1;
                ost--;
            }
            for (int i = 1; i < ProcNum; ++i) {
                if (n % ProcNum > i) {
                    MPI_Recv(&arr[size * i + offset], size + 1,
                        MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else {
                    MPI_Recv(&arr[size * i + offset], size,
                        MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                if (ost > 0) {
                    offset++;
                    ost--;
                }
            }
            if (ProcNum % 2 != 0 && ProcNum != 1)
                ProcNum++;
            ProcNum /= 2;
            size = n / ProcNum;
        }
        quickSortFunction(arr, 0, n - 1);
    } else {
        double* buf = new double[n];
        while (ProcNum > 1) {
            if (ProcRank < ProcNum) {
                if (n % ProcNum > ProcRank)
                    size++;
                MPI_Recv(buf, size, MPI_DOUBLE, 0,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                quickSortFunction(buf, 0, size - 1);
                MPI_Send(buf, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                if (ProcNum % 2 != 0)
                    ProcNum++;
                ProcNum /= 2;
                size = n / ProcNum;
            } else {
                break;
            }
        }
        delete[] buf;
    }
}
