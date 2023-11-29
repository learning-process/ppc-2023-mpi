//  Copyright 2023 Mirzakhmedov Alexander
#ifndef TASKS_TASK_1_MIRZAKHMEDOV_A_NUM_OF_ORDERLY_VIOLATIONS_NUM_VIOLATION_ORDER_VECTOR_H_
#define TASKS_TASK_1_MIRZAKHMEDOV_A_NUM_OF_ORDERLY_VIOLATIONS_NUM_VIOLATION_ORDER_VECTOR_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

std::vector<int> getRandomVector(const int& length);
int getNumViolationOrderVector(std::vector<int> vec);
int getNumViolationOrderVectorParallel(std::vector<int> global_vec,
    int count_size_vector);


#endif  //  TASKS_TASK_1_MIRZAKHMEDOV_A_NUM_OF_ORDERLY_VIOLATIONS_NUM_VIOLATION_ORDER_VECTOR_H_
