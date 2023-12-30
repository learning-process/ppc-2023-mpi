/* Copyright 2023 Belan Vadim */

#ifndef TASKS_TASK_3_BELAN_V_OPTIMIZATION_METHODS_OPTIMIZATION_MPI_H_
#define TASKS_TASK_3_BELAN_V_OPTIMIZATION_METHODS_OPTIMIZATION_MPI_H_

#include <mpi.h>
#include <vector>

// Функция, описывающая реактивное силовое поле
double reactive_force_field(const std::vector<double>& x);

// Функция, вычисляющая значение целевой функции для заданной точки
double objective_function(const std::vector<double>& x);

// Функция, проверяющая, доминирует ли одна точка над другой по Парето
bool dominates(const std::vector<double>& x, const std::vector<double>& y);

// Функция, реализующая мультикритериальный алгоритм глобального поиска
std::vector<std::vector<double>> global_search
(int n, int k, int p, double eps, double r);

#endif  // TASKS_TASK_3_BELAN_V_OPTIMIZATION_METHODS_OPTIMIZATION_MPI_H_
