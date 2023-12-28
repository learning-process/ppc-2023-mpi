// Copyright 2023 Bakhtiarov Alexander
#include "task_3/bakhtiarov_a_multiplication_of_sparse_crs_matrices/multiplication_of_sparse_crs_matrices.h"

double multiLine(std::vector<double> v1, std::vector<double> v2) {
    double ans = 0;
    for (int i = 0; i < v1.size(); ++i) {
        ans += v1[i]*v2[i];
    }
    return ans;
}

CRS multiMatrix(CRS leftMatrix, CRS rightMatrix, int size, int rank) {
    CRS ansMatrix;
    ansMatrix.row = leftMatrix.row;
    ansMatrix.col = rightMatrix.col;
    std::vector<double> line, answer;
    answer.resize(ansMatrix.row*ansMatrix.col);

    std::vector<double> vrow, vcol;
    int step_row = ansMatrix.row  / size;
    int step_end = step_row + (rank == size - 1 ? ansMatrix.row %size: 0);
    std::vector<int>displs, recvcounts;
    if (rank == 0) {
        displs.resize(size);
        recvcounts.resize(size);
        for (int i = 0; i < size; ++i) {
            displs[i] = i*step_row*ansMatrix.col;
            recvcounts[i] = step_row*ansMatrix.col;
        }
        recvcounts[recvcounts.size()-1] += (ansMatrix.row % size) * ansMatrix.col;
    }
    step_row *= rank;
    step_end += step_row;
    for (int row = step_row; row < step_end; ++row) {
        vrow = leftMatrix.getRow(row);
        for (int col = 0; col < ansMatrix.col; ++col) {
            vcol = rightMatrix.getCol(col);
            line.push_back(multiLine(vrow, vcol));
        }
    }
    MPI_Gatherv(
            line.data(),
            line.size(),
            MPI_DOUBLE,
            answer.data(),
            recvcounts.data(),
            displs.data(),
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD);

    if (rank == 0) {
        CRS ans(answer, ansMatrix.row, ansMatrix.col);
        return ans;
    }
    return ansMatrix;
}
