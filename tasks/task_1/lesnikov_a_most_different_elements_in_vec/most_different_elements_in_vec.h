// Copyright 2023 Kulikov Artem
#ifndef TASKS_TASK_1_LESNIKOV_A_MOST_DIFFERENT_ELEMENTS_IN_VEC_MOST_DIFFERENT_ELEMENTS_IN_VEC_H_
#define TASKS_TASK_1_LESNIKOV_A_MOST_DIFFERENT_ELEMENTS_IN_VEC_MOST_DIFFERENT_ELEMENTS_IN_VEC_H_

#include <utility>
#include <algorithm>
#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<int> getRandomVector(int n);
std::vector<std::pair<size_t, size_t>> getSequentialMostDifferentElements(std::vector<int> v);
std::vector<std::pair<size_t, size_t>> getParallelMostDifferentElements(std::vector<int> v, int n);

#endif  // TASKS_TASK_1_LESNIKOV_A_MOST_DIFFERENT_ELEMENTS_IN_VEC_MOST_DIFFERENT_ELEMENTS_IN_VEC_H_
