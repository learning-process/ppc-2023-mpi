// Copyright 2023 by Loginov Maxim
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <utility>
#include <ctime>
#include <random>
#include <vector>

#ifndef TASKS_TASK_3_LOGINOV_M_SORT_BETCHER_SORT_BETCHER_H_
#define TASKS_TASK_3_LOGINOV_M_SORT_BETCHER_SORT_BETCHER_H_

std::vector<int> vec_gen(int size);
int partition(std::vector<int>* vec, int l, int r);
void q_sort(std::vector<int>* vec, int l, int r);
void _allocation(std::vector<int> vec);
void odd_even_merge(std::vector<int> l, std::vector<int> r);
void create_allocation(int _proc_num);
void q_sort_batcher_par(std::vector<int>* vec);

#endif  // TASKS_TASK_3_LOGINOV_M_SORT_BETCHER_SORT_BETCHER_H_
