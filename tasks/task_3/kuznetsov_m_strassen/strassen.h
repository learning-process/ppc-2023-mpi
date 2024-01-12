  // Copyright 2023 Kuznetsov Mikhail
#ifndef TASKS_TASK_3_KUZNETSOV_M_STRASSEN_STRASSEN_H_
#define TASKS_TASK_3_KUZNETSOV_M_STRASSEN_STRASSEN_H_

#include <vector>

std::vector<double> getRandomMatrix(int m, int n);
int findNewMatrixSize(int current_size);
std::vector<double> convertMatrixToRequiredSize(const std::vector<double>& mat, int m, int n, int* new_size);
std::vector<double> sumMatrix(bool isSum, const std::vector<double>& mat1, const std::vector<double>& mat2);
std::vector<std::vector<double>> splitMatrixTo4SubMatrix(const std::vector<double>& mat);
std::vector<double> getStrassenSequence(const std::vector<double>& matA, const std::vector<double>& matB);
std::vector<double> getStrassenParallel(const std::vector<double>& matA, const std::vector<double>& matB, int m, int n);

#endif  // TASKS_TASK_3_KUZNETSOV_M_STRASSEN_STRASSEN_H_
