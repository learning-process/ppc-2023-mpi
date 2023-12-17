// Copyright 2023 Balyasov Ivan
#include "task_3/balyasov_matrix_mult_crs/matrix_mult_crs.h"

SparseMatrix::SparseMatrix(std::vector<std::vector<double>> matrix) {
  countRows = matrix.size();
  countColumns = matrix[0].size();
  rowIndex.push_back(0);
  int NNZ = 0;

  for (size_t i = 0; i < countRows; i++) {
    for (size_t j = 0; j < countColumns; j++) {
      if (matrix[i][j] != 0) {
        values.push_back(matrix[i][j]);
        columnIndex.push_back(j);

        NNZ++;
      }
    }
    rowIndex.push_back(NNZ);
  }
}

SparseMatrix SparseMatrix::Transpose() {
  std::vector<std::vector<size_t>> index(countColumns);
  std::vector<std::vector<double>> values(countColumns);
  for (size_t i = 1; i < rowIndex.size(); ++i)
    for (size_t j = rowIndex[i - 1]; j < rowIndex[i]; ++j) {
      index[columnIndex[j]].push_back(i - 1);
      values[columnIndex[j]].push_back(this->values[j]);
    }
  SparseMatrix res;
  res.countColumns = this->countRows;
  res.countRows = this->countColumns;
  size_t size = 0;
  res.rowIndex.push_back(0);
  for (size_t i = 0; i < countColumns; ++i) {
    for (size_t j = 0; j < index[i].size(); ++j) {
      res.values.push_back(values[i][j]);
      res.columnIndex.push_back(index[i][j]);
    }
    size += index[i].size();
    res.rowIndex.push_back(size);
  }
  return res;
}

void SparseMatrix::PrintStats() {
  std::cout << "Values:" << std::endl;
  for (size_t i = 0; i < values.size(); i++) {
    std::cout << values[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "ColumnIndex:" << std::endl;
  for (size_t i = 0; i < columnIndex.size(); i++) {
    std::cout << columnIndex[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "RowIndex:" << std::endl;
  for (size_t i = 0; i < rowIndex.size(); i++) {
    std::cout << rowIndex[i] << " ";
  }
  std::cout << std::endl;
}

bool SparseMatrix::operator==(SparseMatrix m) const {
  if (this->countColumns != m.countColumns || this->countRows != m.countRows) {
    return false;
  }

  if (this->values.size() != m.values.size()) return false;
  for (size_t i = 0; i < this->values.size(); i++) {
    if (this->values[i] != m.values[i]) {
      return false;
    }
  }

  if (this->columnIndex.size() != m.columnIndex.size()) {
    return false;
  }

  for (size_t i = 0; i < this->columnIndex.size(); i++) {
    if (this->columnIndex[i] != m.columnIndex[i]) {
      return false;
    }
  }

  if (this->rowIndex.size() != m.rowIndex.size()) {
    return false;
  }

  for (size_t i = 0; i < this->rowIndex.size(); i++) {
    if (this->rowIndex[i] != m.rowIndex[i]) {
      return false;
    }
  }
  return true;
}

SparseMatrix seqSparseMatrixMult(const std::vector<std::vector<double>>& _A,
                                 const std::vector<std::vector<double>>& _B) {
  if (_A[0].size() != _B.size()) {
    throw std::string("Non equal sizes matrix");
  }
  SparseMatrix A(_A);
  SparseMatrix transposedB = SparseMatrix(_B).Transpose();
  SparseMatrix result;
  result.countRows = A.countRows;
  result.countColumns = transposedB.countRows;
  result.rowIndex.push_back(0);
  int NNZ = 0;

  for (size_t i = 0; i < A.countRows; i++) {
    bool isNonZero = false;
    for (size_t j = 0; j < transposedB.countRows; j++) {
      double sum = 0;
      int aIndex = A.rowIndex[i];
      int bIndex = transposedB.rowIndex[j];
      while (aIndex < A.rowIndex[i + 1] &&
             bIndex < transposedB.rowIndex[j + 1]) {
        if (A.columnIndex[aIndex] == transposedB.columnIndex[bIndex]) {
          sum += A.values[aIndex] * transposedB.values[bIndex];
          aIndex++;
          bIndex++;
        } else {
          A.columnIndex[aIndex] > transposedB.columnIndex[bIndex] ? bIndex++
                                                                  : aIndex++;
        }
      }
      if (sum > 0) {
        result.values.push_back(sum);
        result.columnIndex.push_back(j);
        isNonZero = true;
        NNZ++;
      }
    }
    if (isNonZero) {
      result.rowIndex.push_back(NNZ);
    }
  }
  return result;
}

SparseMatrix parSparseMatrixMult(const std::vector<std::vector<double>>& _A,
                                 const std::vector<std::vector<double>>& _B) {
  int countProc;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &countProc);
  int countNNZElemsA = 0;

  if (_A[0].size() != _B.size()) {
    throw std::string("Non equal sizes");
  }
  int dataPerProc = _A.size() / countProc;
  int remainder = _A.size() % countProc;

  if (dataPerProc == 0) {
    return seqSparseMatrixMult(_A, _B);
  }

  std::vector<int> sendCountsRowIndex(countProc);
  std::vector<int> displsCountsRowIndex(countProc);

  sendCountsRowIndex[0] = dataPerProc + remainder + 1;
  displsCountsRowIndex[0] = 0;
  for (size_t i = 1; i < countProc; i++) {
    sendCountsRowIndex[i] = dataPerProc + 1;
    displsCountsRowIndex[i] = i * dataPerProc + remainder;
  }

  SparseMatrix localA;
  localA.rowIndex.resize(sendCountsRowIndex[rank]);
  localA.countRows = sendCountsRowIndex[rank] - 1;

  SparseMatrix transposedB = SparseMatrix(_B).Transpose();

  if (rank == 0) {
    SparseMatrix A(_A);
    countNNZElemsA = A.values.size();
    localA.values = A.values;
    localA.columnIndex = A.columnIndex;

    MPI_Scatterv(A.rowIndex.data(), sendCountsRowIndex.data(),
                 displsCountsRowIndex.data(), MPI_INT, localA.rowIndex.data(),
                 sendCountsRowIndex[rank], MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&countNNZElemsA, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(A.values.data(), countNNZElemsA, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(A.columnIndex.data(), countNNZElemsA, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, sendCountsRowIndex.data(),
                 displsCountsRowIndex.data(), MPI_INT, localA.rowIndex.data(),
                 sendCountsRowIndex[rank], MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&countNNZElemsA, 1, MPI_INT, 0, MPI_COMM_WORLD);

    localA.values.resize(countNNZElemsA);
    localA.columnIndex.resize(countNNZElemsA);

    MPI_Bcast(localA.values.data(), countNNZElemsA, MPI_DOUBLE, 0,
              MPI_COMM_WORLD);
    MPI_Bcast(localA.columnIndex.data(), countNNZElemsA, MPI_INT, 0,
              MPI_COMM_WORLD);
  }

  SparseMatrix localResult;
  localResult.countRows = localA.countRows;
  localResult.countColumns = transposedB.countRows;
  int NNZ = 0;

  if (rank == 0) {
    localResult.rowIndex.push_back(0);
  }

  for (size_t i = 0; i < localA.countRows; i++) {
    bool isNonZero = false;
    for (size_t j = 0; j < transposedB.countRows; j++) {
      double sum = 0;
      int aIndex = localA.rowIndex[i];
      int bIndex = transposedB.rowIndex[j];
      while (aIndex < localA.rowIndex[i + 1] &&
             bIndex < transposedB.rowIndex[j + 1]) {
        if (localA.columnIndex[aIndex] == transposedB.columnIndex[bIndex]) {
          sum += localA.values[aIndex] * transposedB.values[bIndex];
          aIndex++;
          bIndex++;
        } else {
          localA.columnIndex[aIndex] > transposedB.columnIndex[bIndex]
              ? bIndex++
              : aIndex++;
        }
      }
      if (sum > 0) {
        localResult.values.push_back(sum);
        localResult.columnIndex.push_back(j);
        isNonZero = true;
        NNZ++;
      }
    }
    if (isNonZero) {
      localResult.rowIndex.push_back(NNZ);
    }
  }

  SparseMatrix result;
  if (rank == 0) {
    result.countRows = _A.size();
    result.countColumns = _B[0].size();
    std::vector<int> nnnzElemsSizes(countProc);
    std::vector<int> rowIndexSizes(countProc);

    int vs = localResult.values.size();
    int ris = localResult.rowIndex.size();

    MPI_Gather(&vs, 1, MPI_INT, nnnzElemsSizes.data(), 1, MPI_INT, 0,
               MPI_COMM_WORLD);
    MPI_Gather(&ris, 1, MPI_INT, rowIndexSizes.data(), 1, MPI_INT, 0,
               MPI_COMM_WORLD);

    std::vector<int> recvCountValues(countProc);
    std::vector<int> recvDisplsValues(countProc);
    std::vector<int> recvCountRowIndex(countProc);
    std::vector<int> recvDisplsRowIndex(countProc);

    recvCountValues[0] = nnnzElemsSizes[0];
    recvDisplsValues[0] = 0;
    recvCountRowIndex[0] = rowIndexSizes[0];
    recvDisplsRowIndex[0] = 0;
    for (size_t i = 1; i < countProc; i++) {
      recvCountValues[i] = nnnzElemsSizes[i];
      recvCountRowIndex[i] = rowIndexSizes[i];
      for (size_t j = 0; j < i; j++) {
        recvDisplsValues[i] += nnnzElemsSizes[j];
        recvDisplsRowIndex[i] += rowIndexSizes[j];
      }
    }

    result.values.resize(recvCountValues[countProc - 1] +
                         recvDisplsValues[countProc - 1]);
    result.columnIndex.resize(recvCountValues[countProc - 1] +
                              recvDisplsValues[countProc - 1]);
    result.rowIndex.resize(recvCountRowIndex[countProc - 1] +
                           recvDisplsRowIndex[countProc - 1]);

    MPI_Bcast(recvCountValues.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvDisplsValues.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvCountRowIndex.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvDisplsRowIndex.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(localResult.values.data(), recvCountValues[rank], MPI_DOUBLE,
                result.values.data(), recvCountValues.data(),
                recvDisplsValues.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Gatherv(localResult.columnIndex.data(), recvCountValues[rank], MPI_INT,
                result.columnIndex.data(), recvCountValues.data(),
                recvDisplsValues.data(), MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(localResult.rowIndex.data(), recvCountRowIndex[rank], MPI_INT,
                result.rowIndex.data(), recvCountRowIndex.data(),
                recvDisplsRowIndex.data(), MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> rIndexResult;
    int add = 0;
    int currentRank = 0;
    for (size_t i = 0; i < result.rowIndex.size(); i++) {
      while (recvCountRowIndex[currentRank] != 0) {
        rIndexResult.push_back(add + result.rowIndex[i]);
        recvCountRowIndex[currentRank]--;
        i++;
      }
      i--;
      add = rIndexResult[rIndexResult.size() - 1];
      currentRank++;
      if (currentRank == countProc) {
        break;
      }
    }
    result.rowIndex = rIndexResult;

    return result;
  } else {
    int vs = localResult.values.size();
    int ris = localResult.rowIndex.size();
    MPI_Gather(&vs, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&ris, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> recvCountValues(countProc);
    std::vector<int> recvDisplsValues(countProc);
    std::vector<int> recvCountRowIndex(countProc);
    std::vector<int> recvDisplsRowIndex(countProc);

    MPI_Bcast(recvCountValues.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvDisplsValues.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvCountRowIndex.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(recvDisplsRowIndex.data(), countProc, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(localResult.values.data(), recvCountValues[rank], MPI_DOUBLE,
                nullptr, recvCountValues.data(), recvDisplsValues.data(),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Gatherv(localResult.columnIndex.data(), recvCountValues[rank], MPI_INT,
                nullptr, recvCountValues.data(), recvDisplsValues.data(),
                MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(localResult.rowIndex.data(), recvCountRowIndex[rank], MPI_INT,
                nullptr, recvCountRowIndex.data(), recvDisplsRowIndex.data(),
                MPI_INT, 0, MPI_COMM_WORLD);
  }

  return SparseMatrix();
}