// Copyright 2023 Kriseev Mikhail
#ifndef TASKS_TASK_3_KRISEEV_M_CSC_MATRIX_MULTIPLICATION_CSC_MATRIX_MULTIPLICATION_H_
#define TASKS_TASK_3_KRISEEV_M_CSC_MATRIX_MULTIPLICATION_CSC_MATRIX_MULTIPLICATION_H_

#include <vector>
#include <boost/serialization/vector.hpp>

struct CscMatrix {
 public:
    std::vector<double> values;
    std::vector<size_t> rowIndices;
    std::vector<size_t> columnPointers;
    size_t rows;
    size_t cols;

    CscMatrix(size_t rows = 0, size_t cols = 0,
              const std::vector<double> &values = {},
              const std::vector<size_t> &rowIndices = {},
              const std::vector<size_t> &columnPointers = {0});
    CscMatrix(const CscMatrix &c);

    bool operator==(const CscMatrix &b) const;

    CscMatrix transpose() const;

    void print() const;

    void setElement(size_t column, size_t row, double value);

    size_t findElementColumn(size_t index) const;
};

CscMatrix multiplyCscMatricesSequential(const CscMatrix &a, const CscMatrix &b);
CscMatrix multiplyCscMatricesParallel(const CscMatrix &a, const CscMatrix &b);

struct MergeMatrices {
    CscMatrix operator()(const CscMatrix &a, const CscMatrix &b);
};

#endif  // TASKS_TASK_3_KRISEEV_M_CSC_MATRIX_MULTIPLICATION_CSC_MATRIX_MULTIPLICATION_H_
