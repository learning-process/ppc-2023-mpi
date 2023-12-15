// Copyright 2023 Saratova Marina
#ifndef TASKS_TASK_3_SARATOVA_M_QUICK_SORT_QUICK_SORT_H_
#define TASKS_TASK_3_SARATOVA_M_QUICK_SORT_QUICK_SORT_H_

#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <memory>
#include <random>
#include <vector>

bool compare_vectors(const std::vector<int>& vec1,
                        const std::vector<int>& vec2);
std::vector<int> seq_sorting(const std::vector<int>& vec);
void generate_random_values(std::vector<int> *vec,
                            int min_value, int max_value);
std::vector<int> task_distribution(const int proc_num, const int task_num);
std::vector<int> par_quick_sort(const std::vector<int>& global_vec);

#endif  // TASKS_TASK_3_SARATOVA_M_QUICK_SORT_QUICK_SORT_H_
