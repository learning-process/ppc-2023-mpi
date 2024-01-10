// Copyright 2023 Korablev Nikita
#ifndef TASKS_TASK_3_KORABLEV_N_MATRIX_MULTIPLICATION_MATRIX_MULT_H_
#define TASKS_TASK_3_KORABLEV_N_MATRIX_MULTIPLICATION_MATRIX_MULT_H_

#include <mpi.h>
#include <iostream>
#include <cstdint>
#include <cmath>

#define UINT64_T uint64_t
#define MIE "mem init error"

template<class T>
class matrix {
 public:
    T* m;
    int numRows, numColums;
    matrix();
    matrix(const matrix<T>& c);
    matrix(const int _numRows, const int _numColums);
    ~matrix();

    T* operator[](const int i);
    const T* operator[](const int i) const;
    matrix<T> operator*(const matrix<T>& c);
    matrix<T> operator+(const matrix<T>& c);
    matrix<T>& operator=(const matrix<T>& c);
    bool operator==(const matrix<T>& c);
    template<class T2> friend std::ostream& operator<< (std::ostream& o,
        const matrix<T2>& c);

    void fillMatrix(int (*generateFunciton)(int, int, int*),
        int value1, int value2, int baseFeedbackValue);
    int getNumRows() const { return numRows; }
    int getNumColums() const { return numColums; }
    T* data() { return m; }
    /*const*/ T* data() const { return /*reinterpret_cast<const T*>(*/m/*)*/; }
    void prepareSpace(int _numRows, int _numColums);
};

template<class T>
matrix<T>::matrix() {
    m = nullptr;
    numRows = numColums = 0;
}

template<class T>
matrix<T>::matrix(const matrix<T>& c) {
    numRows = c.numRows;
    numColums = c.numColums;
    if (numRows * numColums == 0) {
        m = nullptr;
    } else {
        m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
        if (m == nullptr)
            throw MIE;
        for (int i = 0; i < numRows * numColums; i++)
            m[i] = c.m[i];
    }
}

template<class T>
matrix<T>::matrix(const int _numRows, const int _numColumns) {
    if (_numColumns == 0 || _numRows == 0)
        throw "Can't create matrix with 0 column numbers or rows";
    numRows = _numRows;
    numColums = _numColumns;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
    if (m == nullptr)
        throw MIE;
    for (int i = 0; i < numRows * numColums; m[i++] = 0) {}
}

template<class T>
matrix<T>::~matrix() {
    numRows = numColums = 0;
    if (m != nullptr) {
        delete[] m;
        m = nullptr;
    }
}

template<class T>
T* matrix<T>::operator[](const int i) {
    if (i >= numRows)
        throw "Unacceptable row number";
    return m + static_cast<UINT64_T>(i) * numColums;  // C26451
}

template<class T>
const T* matrix<T>::operator[](const int i) const {
    if (i >= numRows)
        throw "Unacceptable row number";
    return reinterpret_cast<const T*>(m +
    static_cast<UINT64_T>(i) * numColums);  // C26451
}

template<class T>
matrix<T> matrix<T>::operator*(const matrix<T>& c) {
    if (numColums != c.numRows)
        throw "Impossible to multiply";
    matrix<T> res(numRows, c.numColums);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < c.numColums; j++) {
            res[i][j] = 0;
            for (int k = 0; k < numColums; k++) {
                res[i][j] += (*this)[i][k] * c[k][j];
            }
        }
    }
    return res;
}

template<class T>
matrix<T> matrix<T>::operator+(const matrix<T>& c) {
    matrix<T> res(numRows, numColums);
    if (numRows != c.numRows || numColums != c.numColums)
        return res;  // throw?
    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numColums; j++)
            res[i][j] = (*this)[i][j] + c[i][j];
    return res;
}

template<class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& c) {
    if (this == &c)
        return *this;
    if (m != nullptr)
        delete[] m;
    numRows = c.numRows;
    numColums = c.numColums;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
    if (m == nullptr)
        throw MIE;
    for (int i = 0; i < numRows * numColums; i++)
        m[i] = c.m[i];
    return *this;
}

template<class T>
bool comp(const matrix<T>& a, const matrix<T>& b) {
  if (a.numRows != b.numRows || a.numColums != b.numColums)
    return false;
  for (int i = 0; i < a.numRows * a.numColums; i++)
    if (a.m[i] != b.m[i])
      return false;
  return true;
}

template<class T>
bool matrix<T>::operator==(const matrix<T>& c) {
    if (numRows != c.numRows || numColums != c.numColums)
        return false;
    for (int i = 0; i < numRows * numColums; i++)
        if (m[i] != c.m[i])
            return false;
    return true;
}

template<class T>
std::ostream& operator<< (std::ostream& o, const matrix<T>& c) {
    for (int i = 0; i < c.getNumRows(); i++) {
        for (int j = 0; j < c.getNumColums(); j++) {
            o << c[i][j] << '\t';
        }
        o << std::endl;
    }
    return o;
}

template<class T>
void matrix<T>::fillMatrix(int (*generateFunciton)(int, int, int*),
int value1, int value2, int baseFeedbackValue) {
    int feedback = baseFeedbackValue;
    for (int i = 0; i < numRows * numColums; i++)
        m[i] = static_cast<T>(generateFunciton(value1, value2, &feedback));
}

template<class T>
void matrix<T>::prepareSpace(int _numRows, int _numColums) {
    if (m != nullptr)
        delete[] m;
    m = nullptr;
    numRows = _numRows;
    numColums = _numColums;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
    if (m == nullptr)
        throw MIE;
}

int generateRngValue(int minValue, int maxValue, int* feedback);
int generateRisingValue(int minValue, int empty, int* feedback);
int generateFallingValue(int empty, int maxValue, int* feedback);
int generateJumpingValue(int v1, int v2, int* feedback);
matrix<double> cannonsMultiplication(matrix<double>* A, matrix<double>* B);


#endif  // TASKS_TASK_3_KORABLEV_N_MATRIX_MULTIPLICATION_MATRIX_MULT_H_
