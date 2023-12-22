// Copyright 2023 Balyasov Ivan
#ifndef TASKS_TASK_3_BALYASOV_MATRIX_MULT_CRS_MATRIX_MULT_CRS_H_
#define TASKS_TASK_3_BALYASOV_MATRIX_MULT_CRS_MATRIX_MULT_CRS_H_

#include <mpi.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class SparseMatrix {
 public:
  std::vector<double> values;
  std::vector<int> columnIndex;
  std::vector<int> rowIndex;
  int countRows;
  int countColumns;

  explicit SparseMatrix(std::vector<std::vector<double>> matrix);

  SparseMatrix Transpose();
  SparseMatrix() = default;
  void PrintStats();

  bool operator==(SparseMatrix m) const;
};

SparseMatrix seqSparseMatrixMult(const std::vector<std::vector<double>>& _A,
                                 const std::vector<std::vector<double>>& _B);
SparseMatrix parSparseMatrixMult(const std::vector<std::vector<double>>& _A,
                                 const std::vector<std::vector<double>>& _B);

#endif  // TASKS_TASK_3_BALYASOV_MATRIX_MULT_CRS_MATRIX_MULT_CRS_H_
