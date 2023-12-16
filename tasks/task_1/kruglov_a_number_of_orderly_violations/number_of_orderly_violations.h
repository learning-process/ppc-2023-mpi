// Copyright 2023 Kruglov Alexey

#ifndef TASKS_TASK_1_KRUGLOV_A_NUMBER_OF_ORDERLY_VIOLATIONS_NUMBER_OF_ORDERLY_VIOLATIONS_H_ 
#define TASKS_TASK_1_KRUGLOV_A_NUMBER_OF_ORDERLY_VIOLATIONS_NUMBER_OF_ORDERLY_VIOLATIONS_H_ 

#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>

int numberOfOrderValids_Sync(const std::vector<int>& Vector);

int numberOfOrderValids_ASync(const std::vector<int>& Vector);

std::vector<int> generateRandomVector(const int& length);

#endif  // TASKS_TASK_1_KRUGLOV_A_NUMBER_OF_ORDERLY_VIOLATIONS_NUMBER_OF_ORDERLY_VIOLATIONS_H_
