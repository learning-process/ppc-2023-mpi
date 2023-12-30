// Copyright 2023 Khodyrev Fedor
#ifndef TASKS_TASK_3_KHODYREV_F_SIMPSON_METHOD_SIMPSON_METHOD_H_
#define TASKS_TASK_3_KHODYREV_F_SIMPSON_METHOD_SIMPSON_METHOD_H_

#include <mpi.h>
#include <vector>
#include <string>

double function_one_dimension(const std::vector<double>& x);

double function_two_dimension(const std::vector<double>& x);

double function_three_dimension(const std::vector<double>& x);

double seq_simpson_method(double (*f)(const std::vector<double>&),
 const std::vector<double>& a, const std::vector<double>& b, int n);

double par_simpson_method(double (*f)(const std::vector<double>&),
const std::vector<double>& a, const std::vector<double>& b, int n);


#endif  // TASKS_TASK_3_KHODYREV_F_SIMPSON_METHOD_SIMPSON_METHOD_H_
