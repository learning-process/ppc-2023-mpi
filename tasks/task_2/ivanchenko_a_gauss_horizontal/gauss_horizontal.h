// Copyright 2023 Ivanchenko Aleksei
#ifndef TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_H_
#define TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_H_

#include <vector>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
using namespace boost::numeric::ublas;
/*
* We will use boost matrix here to simplify the comparison of sequentual and parallel algorithms
* According to docs:
* The templated class matrix<T, F, A> is the base container adaptor for dense matrices.
* For a (m x n)-dimensional matrix and 0 <= i < m, 0 <= j < n every element m(i, j)
* is mapped to the (i x n + j)-th element of the container for row major orientation 
* or the (i + j x m)-th element of the container for column major orientation.
*/

std::vector<int> getMaxParallel(const std::vector<int>& matrix, size_t rows, size_t columns);


vector<double> getRandomVector(size_t size, int minElem = 1, int maxElem = 1e8);
matrix<double> getRandomMatrix(size_t rows, size_t columns, int minElem = 1, int maxElem = 1e8);
vector<double> gaussSequentional(matrix<double> A, vector<double> b);
vector<double> gaussParallel(matrix<double> A, vector<double> b);

#endif  // TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_H_