// Copyright 2023 Kokin Ivan
#ifndef TASKS_TASK_2_KOKIN_I_LENT_HORIZ_SHEMA_LENT_HORIZ_SHEMA_H_
#define TASKS_TASK_2_KOKIN_I_LENT_HORIZ_SHEMA_LENT_HORIZ_SHEMA_H_

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> schemeMult(std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int maxSize,
    int count);
std::vector<int> schemeMultParallel(std::vector<int>* A,
    std::vector<int>* B,
    std::vector<int>* C, int maxSize);

#endif  // TASKS_TASK_2_KOKIN_I_LENT_HORIZ_SHEMA_LENT_HORIZ_SHEMA_H_