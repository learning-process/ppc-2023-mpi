// Copyright 2023 Bakhtiarov Alexander
#pragma once
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>

double multiLine(std::vector<double> v1, std::vector<double> v2);

class CRS {
 public:
    std::vector<double> values;
    std::vector<int> col_indices, row_ptr;
    int row, col;

    std::vector<double> getRow(int indexRow) {
        std::vector<double> line(col);
        for (int i = row_ptr[indexRow]; i < row_ptr[indexRow + 1]; ++i) {
            line[col_indices[i]] = values[i];
        }
        return line;
    }
    std::vector<double> getCol(int indexCol) {
        std::vector<double> line(row);
        for (int i = 0; i < row_ptr.size() - 1; ++i) {
            for (int j = row_ptr[i]; j < row_ptr[i + 1]; ++j) {
                if (col_indices[j] == indexCol) {
                    line[i] = values[j];
                }
            }
        }
        return line;
    }
    CRS() {}
    CRS(std::vector<double> matrix, int _row, int _col){
        col = _col;
        row = _row;
        values.clear();
        col_indices.clear();
        row_ptr.clear();
        int i;
        int val = 0;
        for (i = 0; i < matrix.size(); ++i) {
            if (i%col == 0) {
                row_ptr.push_back(val);
            }
            if (matrix[i] != 0) {
                val+=1;
                values.push_back(matrix[i]);
                col_indices.push_back(i%col);
            }
        }
        row_ptr.push_back(val);
        while (row_ptr.size() < row+1) {
            row_ptr.push_back(row_ptr[row_ptr.size()-1]);
        }
    }
};

CRS multiMatrix(CRS leftMatrix, CRS rightMatrix, int size, int rank);
