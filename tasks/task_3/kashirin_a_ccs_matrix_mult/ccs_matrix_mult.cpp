// Copyright 2023 Kashirin Alexander

#include "task_3/kashirin_a_ccs_matrix_mult/ccs_matrix_mult.h"
#include <mpi.h>
#include <limits>
#include <cmath>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <algorithm>


ClassMatrix::ClassMatrix(const double* matrix, int rows, int cols) {
    clear();
    ccsMatrix.rows = rows;
    ccsMatrix.cols = cols;
    ccsMatrix.vals = 0;
    ccsMatrix.ColPtr = new int[cols + 1];
    ccsMatrix.ColPtr[0] = 0;
    int i, j;
    for (j = 0; j < cols; j++) {
        ccsMatrix.ColPtr[j + 1] = ccsMatrix.ColPtr[j];
        for (i = 0; i < rows; i++) {
            if ( !is_zero(matrix[j + i * cols]) ) {
                ccsMatrix.vals++;
                ccsMatrix.ColPtr[j + 1]++;
            }
        }
    }
    if (ccsMatrix.vals > 0) {
        int k = 0;
        ccsMatrix.RowInd = new int[ccsMatrix.vals];
        ccsMatrix.Values = new double[ccsMatrix.vals];
        for (j = 0; j < cols; j++) {
            for (i = 0; i < rows; i++) {
                int ind = j + i * cols;
                if ( !is_zero(matrix[ind]) ) {
                    ccsMatrix.Values[k] = matrix[ind];
                    ccsMatrix.RowInd[k] = i;
                    k++;
                }
            }
        }
    } else { clear(); }
}

ClassMatrix::ClassMatrix(StructMatrix* props) {
    clear();
    this->ccsMatrix = *props;
}

ClassMatrix& ClassMatrix::operator=(const ClassMatrix& sm) {
    clear();
    ccsMatrix.vals = sm.ccsMatrix.vals;
    ccsMatrix.rows = sm.ccsMatrix.rows;
    ccsMatrix.cols = sm.ccsMatrix.rows;
    ccsMatrix.Values = new double[ccsMatrix.vals];
    ccsMatrix.RowInd = new int[ccsMatrix.vals];
    int i;
    for (i = 0; i < ccsMatrix.vals; i++) {
        ccsMatrix.Values[i] = sm.ccsMatrix.Values[i];
        ccsMatrix.RowInd[i] = sm.ccsMatrix.RowInd[i];
    }
    ccsMatrix.ColPtr = new int[ccsMatrix.cols + 1];
    for (i = 0; i < ccsMatrix.cols + 1; i++) {
        ccsMatrix.ColPtr[i] = sm.ccsMatrix.ColPtr[i];
    }
    return *this;
}

ClassMatrix& ClassMatrix::operator=(ClassMatrix&& sm) {
    std::swap(ccsMatrix, sm.ccsMatrix);
    return *this;
}

ClassMatrix ClassMatrix::operator*(const ClassMatrix& m) const {
    int i, j;
    StructMatrix res;
    res.rows = ccsMatrix.rows;
    res.cols = m.ccsMatrix.cols;
    res.ColPtr = new int[res.cols + 1];
    std::vector<int> temp_rows_ind_arr;
    temp_rows_ind_arr.reserve(std::max(ccsMatrix.vals, m.ccsMatrix.vals));
    std::vector<double> temp_vals_arr;
    temp_vals_arr.reserve(std::max(ccsMatrix.vals, m.ccsMatrix.vals));
    std::memset(res.ColPtr, 0, (res.cols + 1) * sizeof(int));
    ClassMatrix a = TransposeCCS();
    for (j = 0; j < res.cols; j++) {
        for (i = 0; i < res.rows; i++) {
            double sum = 0.0;
            int ks = a.ccsMatrix.ColPtr[i], kf = a.ccsMatrix.ColPtr[i + 1];
            int ls = m.ccsMatrix.ColPtr[j], lf = m.ccsMatrix.ColPtr[j + 1];
            while ((ks < kf) && (ls < lf)) {
                if (a.ccsMatrix.RowInd[ks] < m.ccsMatrix.RowInd[ls]) {
                    ks++;
                } else if (a.ccsMatrix.RowInd[ks] > m.ccsMatrix.RowInd[ls]) {
                    ls++;
                } else {
                    sum += a.ccsMatrix.Values[ks] * m.ccsMatrix.Values[ls];
                    ks++;
                    ls++;
                }
            }
            if (!is_zero(sum)) {
                res.ColPtr[j + 1]++;
                temp_rows_ind_arr.push_back(i);
                temp_vals_arr.push_back(sum);
            }
        }
        res.ColPtr[j + 1] += res.ColPtr[j];
    }
    res.vals = temp_vals_arr.size();
    res.RowInd = new int[res.vals];
    res.Values = new double[res.vals];
    std::copy(temp_rows_ind_arr.begin(), temp_rows_ind_arr.end(), res.RowInd);
    std::copy(temp_vals_arr.begin(), temp_vals_arr.end(), res.Values);
    return ClassMatrix(&res);
}

ClassMatrix ClassMatrix::TransposeCCS() const {
    StructMatrix res;
    res.rows = ccsMatrix.cols;
    res.cols = ccsMatrix.rows;
    res.vals = ccsMatrix.vals;
    res.ColPtr = new int[res.cols + 2];
    res.RowInd = new int[res.vals];
    res.Values = new double[res.vals];
    std::memset(res.ColPtr, 0,
        (res.cols + 2) * sizeof(int));
    int i, j;
    for (i = 0; i < ccsMatrix.vals; i++) {
        res.ColPtr[ccsMatrix.RowInd[i] + 2]++;
    }
    for (i = 2; i < res.cols + 2; i++) {
        res.ColPtr[i] += res.ColPtr[i - 1];
    }
    for (i = 0; i < ccsMatrix.cols; i++) {
        for (j = ccsMatrix.ColPtr[i]; j < ccsMatrix.ColPtr[i + 1]; j++) {
            const int ind = res.ColPtr[ccsMatrix.RowInd[j] + 1]++;
            res.Values[ind] = ccsMatrix.Values[j];
            res.RowInd[ind] = i;
        }
    }
    return ClassMatrix(&res);
}

ClassMatrix MultiplyCCS(ClassMatrix* m1, ClassMatrix* m2) {
    int rank;
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ClassMatrix a; ClassMatrix& m = *m2;
    StructMatrix res;
    res.rows = m1->ccsMatrix.rows;
    res.cols = m2->ccsMatrix.cols;
    int rows = m1->ccsMatrix.rows;
    int i, j;
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, sizeof(m), MPI_BYTE, 0, MPI_COMM_WORLD);
    if (rank == 0) { a = m1->TransposeCCS(); }
    MPI_Bcast(&a, sizeof(a), MPI_BYTE, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        a.ccsMatrix.Values = new double[a.ccsMatrix.vals];
        a.ccsMatrix.RowInd = new int[a.ccsMatrix.vals];
        a.ccsMatrix.ColPtr = new int[a.ccsMatrix.cols + 1];
        m.ccsMatrix.Values = new double[m.ccsMatrix.vals];
        m.ccsMatrix.RowInd = new int[m.ccsMatrix.vals];
        m.ccsMatrix.ColPtr = nullptr;
    }
    MPI_Bcast(a.ccsMatrix.Values, a.ccsMatrix.vals, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(a.ccsMatrix.RowInd, a.ccsMatrix.vals, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(m.ccsMatrix.Values, m.ccsMatrix.vals, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(m.ccsMatrix.RowInd, m.ccsMatrix.vals, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a.ccsMatrix.ColPtr, a.ccsMatrix.cols + 1, MPI_INT, 0, MPI_COMM_WORLD);
    int chunkSize = (m.ccsMatrix.cols) / numProc;
    int remain = (m.ccsMatrix.cols) % numProc;
    int cols = (rank == 0) ? chunkSize + remain : chunkSize;
    if (rank == 0) {
        for (i = 1; i < numProc; i++) {
            MPI_Send(m.ccsMatrix.ColPtr + remain + i * (chunkSize), chunkSize + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        m.ccsMatrix.ColPtr = new int[chunkSize + 1];
        MPI_Recv(m.ccsMatrix.ColPtr, chunkSize + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    std::vector<int> tmpRows;
    std::vector<double> tmpValues;
    int* tmpColPtr = new int[cols + 1];
    std::memset(tmpColPtr, 0, (cols + 1) * sizeof(int));
    for (j = 0; j < cols; j++) {
        for (i = 0; i < rows; i++) {
            double sum = 0.0;
            int ks = a.ccsMatrix.ColPtr[i], kf = a.ccsMatrix.ColPtr[i + 1];
            int ls = m.ccsMatrix.ColPtr[j], lf = m.ccsMatrix.ColPtr[j + 1];
            while ((ks < kf) && (ls < lf)) {
                if (a.ccsMatrix.RowInd[ks] < m.ccsMatrix.RowInd[ls]) {
                    ks++;
                } else if (a.ccsMatrix.RowInd[ks] > m.ccsMatrix.RowInd[ls]) {
                    ls++;
                } else {
                    sum += a.ccsMatrix.Values[ks] * m.ccsMatrix.Values[ls];
                    ks++;
                    ls++;
                }
            }
            if (!is_zero(sum)) {
                tmpColPtr[j + 1]++;
                tmpRows.push_back(i);
                tmpValues.push_back(sum);
            }
        }
        tmpColPtr[j + 1] += tmpColPtr[j];
    }
    int temp_vals = tmpValues.size();
    a.clear();
    int* res_vals;
    if (rank == 0) { res_vals = new int[numProc]; } else { m.clear(); }
    MPI_Gather(&temp_vals, 1, MPI_INT, res_vals, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (i = 0; i < numProc; i++) {
            res.vals += res_vals[i];
        }
        res.Values = new double[res.vals];
        res.RowInd = new int[res.vals];
        res.ColPtr = new int[res.cols + 1];
        std::copy(tmpValues.begin(), tmpValues.end(), res.Values);
        std::copy(tmpRows.begin(), tmpRows.end(), res.RowInd);
        std::copy(tmpColPtr, tmpColPtr + cols + 1, res.ColPtr);
        delete[] tmpColPtr;
        int offset = res_vals[0];
        for (i = 1; i < numProc; i++) {
            int ind = remain + (chunkSize)*i + 1;
            MPI_Recv(res.Values + offset, res_vals[i], MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(res.RowInd + offset, res_vals[i], MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(res.ColPtr + ind, chunkSize, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (j = ind; j < ind + chunkSize; j++) {
                res.ColPtr[j] += res.ColPtr[ind - 1];
            }
            offset += res_vals[i];
        }
        delete[] res_vals;
    } else {
        MPI_Send(tmpValues.data(), temp_vals, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(tmpRows.data(), temp_vals, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(tmpColPtr + 1, chunkSize, MPI_INT, 0, 2, MPI_COMM_WORLD);
        delete[] tmpColPtr;
    }
    return ClassMatrix(&res);
}

void ClassMatrix::clear() {
    if (ccsMatrix.Values != nullptr) { delete[] ccsMatrix.Values; }
    if (ccsMatrix.RowInd != nullptr) { delete[] ccsMatrix.RowInd; }
    if (ccsMatrix.ColPtr != nullptr) { delete[] ccsMatrix.ColPtr; }
    ccsMatrix.vals = 0;
    ccsMatrix.rows = 0;
    ccsMatrix.cols = 0;
    ccsMatrix.Values = nullptr;
    ccsMatrix.RowInd = nullptr;
    ccsMatrix.ColPtr = nullptr;
}

ClassMatrix::~ClassMatrix() {
    clear();
}
