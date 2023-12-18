// Copyright 2023 Benduyzhko Tatiana
#ifndef TASKS_TASK_1_BENDUYZHKO_MIN_ELEMENTS_MIN_ELEMENTS_H_
#define TASKS_TASK_1_BENDUYZHKO_MIN_ELEMENTS_MIN_ELEMENTS_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector, const std::string& ops);
int getSequentialOperations(std::vector<int> vec, const std::string& ops);

#endif  // TASKS_TASK_1_BENDUYZHKO_MIN_ELEMENTS_MIN_ELEMENTS_H_
