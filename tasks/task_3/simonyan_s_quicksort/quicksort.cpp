// Copyright 2023 Suren Simonyan

#include <algorithm>
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
double* merge(double* arr1, int n1, double* arr2, int n2) {
    double* result = new double[n1 + n2];
    int i = 0;
    int j = 0;
    int k;
    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            result[k] = arr2[j];
            j++;
        } else if (j >= n2) {
            result[k] = arr1[i];
            i++;
        } else if (arr1[i] < arr2[j]) {
            result[k] = arr1[i];
            i++;
        } else {
            result[k] = arr2[j];
            j++;
        }
    }
    return result;
}

int getothersize(int n, int step, int r) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int gs = n / ProcNum;
    if (r >= ProcNum)
        return 0;
    if (r == ProcNum - 1) {
        return gs;
    }
    if (r < n % ProcNum) {
        gs++;
    }
    for (int s = 1; s < step; s = s * 2) {
        gs = gs + getothersize(n, s, s + r);
    }
    return gs;
}

double* Sort(double* arr, int n) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int size = n / ProcNum;
    int ost = n % ProcNum;
    int offset = std::min(ProcRank, ost);
    int position = ProcRank * size + offset;
    if (ost > ProcRank)
        size++;
    double* buf = new double[size];
    memcpy(buf, &arr[position], sizeof(double) * size);
    double* data = new double[n];
    if (ProcRank == 0) {
        if (ost > 0) {
            ost--;
            offset++;
        }
        for (int i = 1; i < ProcNum; ++i) {
            if (n % ProcNum > i) {
                MPI_Send(&arr[n / ProcNum * i + offset],
                    n / ProcNum + 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            } else {
                MPI_Send(&arr[n / ProcNum * i + offset],
                    n / ProcNum, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
            if (ost > 0) {
                offset++;
                ost--;
            }
        }
    } else {
        MPI_Recv(buf, size, MPI_DOUBLE, 0,
            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    quickSortFunction(buf, 0, size - 1);
    for (int step = 1; step < ProcNum; step = step * 2) {
        if ((ProcRank % (2 * step) != 0) && (ProcRank - step >= 0)) {
            MPI_Send(buf, size, MPI_DOUBLE,
                ProcRank - step, 0,
                MPI_COMM_WORLD);
            break;
        }
        if ((ProcRank + step < ProcNum)) {
            int othersizeofbuf = getothersize(n, step, ProcRank + step);
            double* recvbuf = new double[othersizeofbuf];
            MPI_Recv(recvbuf, othersizeofbuf,
                MPI_DOUBLE, ProcRank + step, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            data = merge(buf, size, recvbuf, othersizeofbuf);
            delete[] recvbuf;
            delete[] buf;
            size = size + othersizeofbuf;
            buf = new double[size];
            memcpy(buf, data, size * sizeof(double));
            delete[] data;
        }
    }
    data = new double[n];
    if (ProcRank == 0) {
        data = buf;
        for (int i = 1; i < ProcNum; ++i) {
            MPI_Send(data, n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(data, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return data;
}
