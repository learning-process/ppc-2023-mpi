// Copyright 2023 Pivovarov Alexey

#ifndef TASKS_TASK_3_PIVOVAROV_A_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_SIMPLE_MERGE_H_
#define TASKS_TASK_3_PIVOVAROV_A_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_SIMPLE_MERGE_H_

#include <mpi.h>
#include <vector>
#include <algorithm>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

std::vector<int> generate_random_array(int length, int minValue, int maxValue);
void merge(std::vector<int> arr, int low, int mid, int high);
void shell_sort_seq(std::vector<int> arr);
void shell_sort_par(std::vector<int> arr, const boost::mpi::communicator& world);

#endif  // TASKS_TASK_3_PIVOVAROV_A_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_SIMPLE_MERGE_H_
