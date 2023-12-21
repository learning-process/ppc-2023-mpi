// Copyright 2023 Skotin Alexander
#ifndef TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_MIN_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_MIN_OF_VECTOR_ELEMENTS_H_

#include <vector>

int findMinElementMPI(const std::vector<int>& vec, int vecSize);
std::vector<int> generateRandomVector(int size, int minElem = -500,
                                      int maxElem = 500);

#endif  // TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_MIN_OF_VECTOR_ELEMENTS_H_
