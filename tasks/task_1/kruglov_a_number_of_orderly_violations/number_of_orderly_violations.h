// Copyright 2023 Kruglov Alexey

#ifndef TASKS_TASK_1_KRUGLOV_A_NUM_OF_ORDERLY_VIOLATIONS_NUMBER_OF_ORDERLY_VIOLATIONS_H //filenames with full path
#define TASKS_TASK_1_KRUGLOV_A_NUM_OF_ORDERLY_VIOLATIONS_NUMBER_OF_ORDERLY_VIOLATIONS_H

#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>

//template<typename T>
//int numberOfOrderValids_Sync(const std::vector<T>& Vector);
//template<typename T>
//int numberOfOrderValids_ASync(const std::vector<T>& Vector);
//template<typename T>
//std::vector<T> generateRandomVector(const int& length);

int numberOfOrderValids_Sync(const std::vector<int>& Vector);

int numberOfOrderValids_ASync(const std::vector<int>& Vector);

std::vector<int> generateRandomVector(const int& length);

#endif  // TASKS_TASK_1_KRUGLOV_A_NUM_OF_ORDERLY_VIOLATIONS_NUMBER_OF_ORDERLY_VIOLATIONS_H