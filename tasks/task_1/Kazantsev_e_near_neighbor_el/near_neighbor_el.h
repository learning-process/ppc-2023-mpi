// Copyright 2023 Nesterov Alexander
#ifndef TASKS_TASK_1_KAZANTSEV_E_NEAR_NEIGHBOR_EL_NEAR_NEIGHBOR_EL_H_
#define TASKS_TASK_1_KAZANTSEV_E_NEAR_NEIGHBOR_EL_NEAR_NEIGHBOR_EL_H_

#include <vector>
#include <string>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::vector<int> getRandVec(int size);
int SequentialCount(const std::vector<int>& vec);
int parallelCount(const std::vector<int>& vec, int sz);
#endif  // TASKS_TASK_1_KAZANTSEV_E_NEAR_NEIGHBOR_EL_NEAR_NEIGHBOR_EL_H_
