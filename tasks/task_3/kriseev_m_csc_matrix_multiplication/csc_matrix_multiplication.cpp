// Copyright 2023 Kriseev Mikhail
#include "task_3/kriseev_m_csc_matrix_multiplication/csc_matrix_multiplication.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {
template <class Archive>
void serialize(Archive &ar, CscMatrix &m,     // NOLINT
               const unsigned int version) {  // NOLINT
    ar & m.rows;
    ar & m.cols;
    ar & m.values;
    ar & m.rowIndices;
    ar & m.columnPointers;
}
}  // namespace serialization
}  // namespace boost

CscMatrix::CscMatrix(size_t rows, size_t cols,
                     const std::vector<double> &values,
                     const std::vector<size_t> &rowIndices,
                     const std::vector<size_t> &columnPointers)
    : rows(rows),
      cols(cols),
      values(values),
      rowIndices(rowIndices),
      columnPointers(columnPointers) {}
CscMatrix::CscMatrix(const CscMatrix &c)
    : rows(c.rows),
      cols(c.cols),
      values(c.values),
      rowIndices(c.rowIndices),
      columnPointers(c.columnPointers) {}
bool CscMatrix::operator==(const CscMatrix &b) const {
    if (b.rows != rows || b.cols != cols ||
        b.rowIndices.size() != rowIndices.size()) {
        return false;
    }
    for (size_t i = 0; i < b.rowIndices.size(); ++i) {
        if (std::abs(values[i] - b.values[i]) >
            std::numeric_limits<double>::epsilon()) {
            return false;
        }
        if (rowIndices[i] != b.rowIndices[i]) {
            return false;
        }
    }
    for (size_t c = 0; c < b.columnPointers.size(); ++c) {
        if (columnPointers[c] != b.columnPointers[c]) {
            return false;
        }
    }
    return true;
}

CscMatrix CscMatrix::transpose() const {
    CscMatrix n = {this->cols, this->rows};
    if (this->rowIndices.empty()) {
        n.columnPointers.push_back(0);
        return n;
    }
    size_t maxRow =
        *std::max_element(this->rowIndices.begin(), this->rowIndices.end());
    for (size_t i = 0; i <= maxRow; ++i) {
        for (size_t j = 0; j < this->rowIndices.size(); ++j) {
            if (this->rowIndices[j] == i) {
                n.values.push_back(this->values[j]);
                n.rowIndices.push_back(this->findElementColumn(j));
            }
        }
        n.columnPointers.push_back(n.rowIndices.size());
    }
    return n;
}

void CscMatrix::print() const {
    std::cout << "v: ";
    for (auto v : values) {
        std::cout << v << " ";
    }
    std::cout << "r: ";
    for (auto v : rowIndices) {
        std::cout << v << " ";
    }
    std::cout << "c: ";
    for (auto v : columnPointers) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}
void CscMatrix::setElement(size_t column, size_t row, double value) {
    size_t beginCol = columnPointers[column];
    size_t endCol = columnPointers[column + 1];
    size_t insertIndex = std::lower_bound(rowIndices.begin() + beginCol,
                                          rowIndices.begin() + endCol, row) -
                         rowIndices.begin();
    if (!rowIndices.empty() && row == rowIndices[insertIndex]) {
        values[insertIndex] = value;
        return;
    }

    values.insert(values.begin() + insertIndex, value);
    rowIndices.insert(rowIndices.begin() + insertIndex, row);

    for (size_t c = column + 1; c < columnPointers.size(); ++c) {
        columnPointers[c]++;
    }
}
size_t CscMatrix::findElementColumn(size_t index) const {
    for (size_t i = 0; i < columnPointers.size() - 1; i++) {
        if (index >= columnPointers[i] && index < columnPointers[i + 1]) {
            return i;
        }
    }
    return columnPointers.size();
}

CscMatrix multiplyCscMatricesParallel(const CscMatrix &a, const CscMatrix &b) {
    boost::mpi::communicator world;

    if (world.size() == 1) {
        return multiplyCscMatricesSequential(a, b);
    }

    if (a.cols != b.rows) {
        throw std::invalid_argument("b");
    }
    CscMatrix a_t(a.cols, a.rows);
    CscMatrix b_c(b);

    a_t = a.transpose();
    CscMatrix res = {a_t.cols, b.cols, {}, {}, {0}};

    std::vector<size_t> tempCol(a_t.rows, -1);

    std::vector<size_t> j_indices(
        res.cols / world.size() +
        (world.rank() < res.cols % world.size() ? 1 : 0));
    if (world.rank() == 0) {
        std::vector<int> i_sizes(world.size(), res.rows / world.size());
        for (size_t k = 0; k < res.rows % world.size(); ++k) {
            i_sizes[k]++;
        }
        std::vector<int> j_sizes(world.size(), res.cols / world.size());
        for (size_t k = 0; k < res.cols % world.size(); ++k) {
            j_sizes[k]++;
        }
        std::vector<size_t> all_j_indices(res.cols);
        std::iota(all_j_indices.begin(), all_j_indices.end(), 0);

        boost::mpi::scatterv(world, all_j_indices, j_sizes, j_indices.data(),
                             0);
    } else {
        boost::mpi::scatterv(world, j_indices.data(), j_indices.size(), 0);
    }

    for (size_t j = 0; j < res.cols; ++j) {
        for (size_t i = 0; i < res.rows; ++i) {
            if (std::find(j_indices.begin(), j_indices.end(), j) ==
                j_indices.end()) {
                continue;
            }
            double dot = 0;
            size_t colSizeA = a_t.columnPointers[i + 1] - a_t.columnPointers[i];
            size_t colSizeB = b_c.columnPointers[j + 1] - b_c.columnPointers[j];
            size_t beginColA = a_t.columnPointers[i];
            size_t beginColB = b_c.columnPointers[j];
            tempCol.assign(tempCol.size(), -1);
            for (size_t k = 0; k < colSizeA; ++k) {
                size_t l = a_t.rowIndices[beginColA + k];
                tempCol[l] = k;
            }
            for (size_t k = 0; k < colSizeB; ++k) {
                size_t l = b_c.rowIndices[beginColB + k];
                if (tempCol[l] == -1) {
                    continue;
                }
                dot += a_t.values[beginColA + tempCol[l]] *
                       b_c.values[beginColB + k];
            }
            if (dot != 0) {
                res.values.push_back(dot);
                res.rowIndices.push_back(i);
            }
        }
        res.columnPointers.push_back(res.values.size());
    }
    CscMatrix finalRes(res.rows, res.cols);
    boost::mpi::reduce(world, res, finalRes, MergeMatrices(), 0);
    return finalRes;
}

CscMatrix MergeMatrices::operator()(const CscMatrix &a, const CscMatrix &b) {
    CscMatrix res(a);

    size_t col = 0;
    res.rowIndices.reserve(res.rowIndices.size() + b.rowIndices.size());
    res.values.reserve(res.values.size() + b.values.size());

    for (size_t i = 0; i < b.columnPointers.size() - 1; ++i) {
        auto colSize = b.columnPointers[i + 1] - b.columnPointers[i];
        if (colSize) {
            for (size_t j = 0; j < colSize; ++j) {
                res.values.insert(
                    res.values.begin() + res.columnPointers[i] + j,
                    b.values[b.columnPointers[i] + j]);
                res.rowIndices.insert(
                    res.rowIndices.begin() + +res.columnPointers[i] + j,
                    b.rowIndices[b.columnPointers[i] + j]);
            }
            for (size_t k = i + 1; k < res.columnPointers.size(); ++k) {
                res.columnPointers[k] += colSize;
            }
        }
    }

    return res;
}

CscMatrix multiplyCscMatricesSequential(const CscMatrix &a,
                                        const CscMatrix &b) {
    if (a.cols != b.rows) {
        throw std::invalid_argument("b");
    }
    CscMatrix a_t = a.transpose();
    CscMatrix res = {a.rows, b.cols, {}, {}, {0}};
    std::vector<size_t> tempCol(a.cols, -1);
    for (size_t j = 0; j < res.cols; ++j) {
        for (size_t i = 0; i < res.rows; ++i) {
            double dot = 0;
            size_t colSizeA = a_t.columnPointers[i + 1] - a_t.columnPointers[i];
            size_t colSizeB = b.columnPointers[j + 1] - b.columnPointers[j];
            size_t beginColA = a_t.columnPointers[i];
            size_t beginColB = b.columnPointers[j];
            tempCol.assign(tempCol.size(), -1);

            for (size_t k = 0; k < colSizeA; ++k) {
                size_t l = a_t.rowIndices[beginColA + k];
                tempCol[l] = k;
            }
            for (size_t k = 0; k < colSizeB; ++k) {
                size_t l = b.rowIndices[beginColB + k];
                if (tempCol[l] == -1) {
                    continue;
                }
                dot += a_t.values[beginColA + tempCol[l]] *
                       b.values[beginColB + k];
            }

            if (dot != 0) {
                res.values.push_back(dot);
                res.rowIndices.push_back(i);
            }
        }
        res.columnPointers.push_back(res.values.size());
    }
    return res;
}
