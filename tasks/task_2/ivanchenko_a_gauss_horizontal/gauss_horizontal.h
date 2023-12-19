// Copyright 2023 Ivanchenko Aleksei
#ifndef TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
#define TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_

#include <vector>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
/*
* We will use boost matrix here to simplify the comparison of sequentual and parallel algorithms
* According to docs:
* The templated class matrix<T, F, A> is the base container adaptor for dense matrices.
* For a (m x n)-dimensional matrix and 0 <= i < m, 0 <= j < n every element m(i, j)
* is mapped to the (i x n + j)-th element of the container for row major orientation 
* or the (i + j x m)-th element of the container for column major orientation.
*/

boost::numeric::ublas::vector<double>getRandomVector(size_t size, int minElem = 1, int maxElem = 1e8);
boost::numeric::ublas::matrix<double>getRandomMatrix(size_t rows, size_t columns, int minElem = 1, int maxElem = 1e8);
boost::numeric::ublas::vector<double>gaussSequentional(boost::numeric::ublas::matrix<double>A,
 boost::numeric::ublas::vector<double>b);
boost::numeric::ublas::vector<double>gaussParallel(boost::numeric::ublas::matrix<double>A,
 boost::numeric::ublas::vector<double>b);

#endif  // TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
