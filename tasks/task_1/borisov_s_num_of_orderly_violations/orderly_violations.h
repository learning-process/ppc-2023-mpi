// Copyright 2023 Borisov Saveliy
#ifndef TASKS_TASK_1_BORISOV_S_NUM_OF_ORDERLY_VIOLATIONS_ORDERLY_VIOLATIONS_H_
#define TASKS_TASK_1_BORISOV_S_NUM_OF_ORDERLY_VIOLATIONS_ORDERLY_VIOLATIONS_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int  size);
int getViolationsInParallel(std::vector<int> vector, int vector_size);
int getViolationsSequentially(std::vector<int> vector);

#endif  // TASKS_TASK_1_BORISOV_S_NUM_OF_ORDERLY_VIOLATIONS_ORDERLY_VIOLATIONS_H_
