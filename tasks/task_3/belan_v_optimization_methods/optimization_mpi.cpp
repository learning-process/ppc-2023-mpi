/* Copyright 2023 Belan Vadim */

#include <mpi.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <vector>
#include "task_3/belan_v_optimization_methods/optimization_mpi.h"

// Функция, описывающая реактивное силовое поле
double reactive_force_field(const std::vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += xi * xi;
    }
    return sum;
}

// Функция, вычисляющая значение целевой функции для заданной точки
double objective_function(const std::vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += std::abs(xi);
    }
    return sum;
}

// Функция, проверяющая, доминирует ли одна точка над другой по Парето
bool dominates(const std::vector<double>& x, const std::vector<double>& y) {
    // Предполагаем, что обе точки имеют одинаковую размерность
    int n = x.size();
    // Проверяем, что x не хуже y по всем критериям
    bool better = false;
    for (int i = 0; i < n; i++) {
        if (x[i] > y[i]) {
            return false;
        }
        if (x[i] < y[i]) {
            better = true;
        }
    }
    // Проверяем, что x лучше y хотя бы по одному критерию
    return better;
}

// Функция, реализующая мультикритериальный алгоритм глобального поиска
std::vector<std::vector<double>> global_search
(int n, int k, int p, double eps, double r) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::mt19937 gen(rank);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    // Множество Парето-оптимальных точек
    std::vector<std::vector<double>> pareto_set;

    // Начальное множество точек
    std::vector<std::vector<double>> initial_set(p);

    // Генерируем начальное множество точек на каждом процессе
    for (int i = 0; i < p; i++) {
        initial_set[i].resize(n);
        for (int j = 0; j < n; j++) {
            initial_set[i][j] = dist(gen);
        }
    }

    // Синхронизируем начальное множество точек между процессами
    for (int i = 0; i < p; i++) {
        MPI_Bcast
        (initial_set[i].data(), n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    }

    // Основной цикл алгоритма
    for (int t = 0; t < k; t++) {
        // Выбираем случайную точку из начального множества
        int index = std::uniform_int_distribution<int>(0, p - 1)(gen);
        std::vector<double> x = initial_set[index];

        // Генерируем новую точку в окрестности выбранной
        std::vector<double> y(n);
        for (int i = 0; i < n; i++) {
            y[i] = x[i] + r * dist(gen);
        }

        // Вычисляем значения функций для новой точки
        double f1 = reactive_force_field(y);
        double f2 = objective_function(y);

        // Собираем значения функций со всех процессов
        std::vector<double> f1_all(size);
        std::vector<double> f2_all(size);

        MPI_Allgather(&f1, 1,
        MPI_DOUBLE, f1_all.data(), 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Allgather(&f2, 1,
        MPI_DOUBLE, f2_all.data(), 1, MPI_DOUBLE, MPI_COMM_WORLD);

        // Находим лучшую точку среди сгенерированных
        int best = 0;
        for (int i = 1; i < size; i++) {
            if (f1_all[i] < f1_all[best] ||
             (f1_all[i] == f1_all[best] && f2_all[i] < f2_all[best])) {
                best = i;
            }
        }

        // Обновляем начальное множество точек
        initial_set[index] = y;

        // Обновляем множество Парето-оптимальных точек
        bool add = true;
        for (auto it = pareto_set.begin(); it != pareto_set.end();) {
            if (dominates(*it, y)) {
                add = false;
                break;
            }
            if (dominates(y, *it)) {
                it = pareto_set.erase(it);
            } else {
                // Иначе переходим к следующей точке
                ++it;
            }
        }
        if (add) {
            // Добавляем новую точку в множество Парето-оптимальных
            pareto_set.push_back(y);
        }

        // Проверяем условие остановки
        if (r < eps) {
            break;
        }

        // Уменьшаем параметр разброса
        r *= 0.9;
    }

    return pareto_set;
}
