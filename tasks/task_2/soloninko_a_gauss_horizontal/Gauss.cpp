// Copyright 2023 Soloninko Andrey

#include "task_2/soloninko_a_gauss_horizontal/Gauss.h"

#include <iostream>
#include <random>
#include <vector>

bool check_res(std::vector<double>* matrix,
               std::vector<double>* res, int size) {
    for (int i = 0; i < size; i++) {
        double sum = 0;
        for (int j = 0; j < size; j++)
            sum += (*matrix)[i * (size + 1) + j] * (*res)[j];

        if (std::abs(sum - (*matrix)[i * (size + 1) + size]) > 0.0001)
            return false;
    }
    return true;
}

void rand_matr(std::vector<double>* matrix, int size) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(-100, 100);
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size + 1; j++) (*matrix)[i * (size + 1) + j] = unif(rd);
}

void gaussian_elimination(std::vector<double>* matrix, std::vector<int>* rows,
                          int size, int rank, int comm_size, int row,
                          int numsegment) {
    std::vector<double> tmp_matr(size + 1);
    for (int i = 0; i < size - 1; i++) {
        if (i == (*rows)[row]) {
            MPI_Bcast(&(*matrix).data()[(*rows)[row] * (size + 1)], size + 1,
                      MPI_DOUBLE, rank, MPI_COMM_WORLD);
            for (int j = 0; j <= size; j++)
                tmp_matr[j] = (*matrix)[(*rows)[row] * (size + 1) + j];
            row++;
        } else {
            MPI_Bcast(tmp_matr.data(), size + 1, MPI_DOUBLE, i % comm_size,
                      MPI_COMM_WORLD);
        }
        for (int j = row; j < numsegment; j++) {
            double mult_ratio = (*matrix)[(*rows)[j] * (size + 1) + i] / tmp_matr[i];
            for (int k = i; k < size + 1; k++)
                (*matrix)[(*rows)[j] * (size + 1) + k] -= mult_ratio * tmp_matr[k];
        }
    }
}

void gaussian_substitution(const std::vector<double>& matrix,
                           std::vector<double>* res,
                           const std::vector<int>& rows, int row,
                           int numsegment, int rank, int comm_size, int size) {
    row = 0;
    for (int i = 0; i < size; i++) {
        if (i == rows[row]) {
            (*res)[i] = matrix[i * (size + 1) + size];
            row++;
        }
    }
    row = numsegment - 1;
    for (int i = size - 1; i > 0; i--) {
        if (row >= 0) {
            if (i == rows[row]) {
                (*res)[i] /= matrix[i * (size + 1) + i];
                MPI_Bcast(&((*res).data())[i], 1, MPI_DOUBLE, rank,
                          MPI_COMM_WORLD);
                row--;
            } else {
                MPI_Bcast(&((*res).data())[i], 1, MPI_DOUBLE, i % comm_size,
                          MPI_COMM_WORLD);
            }
        } else {
            MPI_Bcast(&((*res).data())[i], 1, MPI_DOUBLE, i % comm_size,
                      MPI_COMM_WORLD);
        }

        for (int j = 0; j <= row; j++)
            (*res)[rows[j]] -= matrix[rows[j] * (size + 1) + i] * (*res)[i];
    }
    if (rank == 0) (*res)[0] /= matrix[rows[row] * (size + 1)];
}

void gauss_algo(std::vector<double>* matrix, std::vector<double>* res,
                int size) {
    int rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    int numsegment, rows_div;

    rows_div = size / comm_size;
    if (size % comm_size) rows_div++;

    numsegment = rows_div;
    if (rank >= comm_size - (comm_size * rows_div - size))
        numsegment = rows_div - 1;

    std::vector<int> rows(numsegment);

    for (int i = 0; i < numsegment; i++) rows[i] = rank;

    MPI_Bcast((*matrix).data(), size * (size + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int row = 0;
    gaussian_elimination(matrix, &rows, size, rank, comm_size, row, numsegment);

    gaussian_substitution((*matrix), res, rows, row, numsegment, rank, comm_size, size);
}
