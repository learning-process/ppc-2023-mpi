// Copyright 2023 Sadikov Damir
#ifndef TASKS_TASK_2_SADIKOV_D_ITERATIVE_METHOD_ITERATIVE_METHOD_H_
#define TASKS_TASK_2_SADIKOV_D_ITERATIVE_METHOD_ITERATIVE_METHOD_H_

#include <vector>

std::vector<double> getRandomVector(int n);
std::vector<double> getRandomMatrix(int n);
double NormOfDifference(const std::vector<double>& a, const std::vector<double>& b);
std::vector<double> SequentialIter(const std::vector<double>& A, const std::vector<double>& b, int n);
std::vector<double> ParallelIter(const std::vector<double>& A, const std::vector<double>& b, int n);

#endif  // TASKS_TASK_2_SADIKOV_D_ITERATIVE_METHOD_ITERATIVE_METHOD_H_
