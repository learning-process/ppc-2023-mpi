// Copyright 2023 Balyasov Ivan
#ifndef TASKS_TASK_1_BALYASOV_NUM_OF_ALTERNATIONS_SIGNS_NUM_OF_ALTERNATIONS_SIGNS_H_
#define TASKS_TASK_1_BALYASOV_NUM_OF_ALTERNATIONS_SIGNS_NUM_OF_ALTERNATIONS_SIGNS_H_

#include <mpi.h>

#include <random>
#include <vector>

int seq_num_of_alternations_signs(const std::vector<int>& elems);
int par_num_of_alternations_signs(const std::vector<int>& elems);

#endif  // TASKS_TASK_1_BALYASOV_NUM_OF_ALTERNATIONS_SIGNS_NUM_OF_ALTERNATIONS_SIGNS_H_
