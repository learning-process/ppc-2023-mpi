// Copyright 2023 Mamaeva Olga
#ifndef TASKS_TASK_3_MAMEVA_O_SIMPSON_METHOD_KURGUZIKOV_SIMPSON_METHOD_H_
#define TASKS_TASK_3_MAMEVA_O_SIMPSON_METHOD_KURGUZIKOV_SIMPSON_METHOD_H_
#include <vector>
#include <algorithm>
#include <utility>

double ParallelVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr);
double SequentialVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr);

#endif  // TASKS_TASK_3_MAMEVA_O_SIMPSON_METHOD_KURGUZIKOV_SIMPSON_METHOD_H_
