// Copyright 2023 Kostin Artem
#ifndef TASKS_TASK_1_KOSTIN_A_AVERAGE_OF_VECTOR_ELEMENTS_AVERAGE_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_KOSTIN_A_AVERAGE_OF_VECTOR_ELEMENTS_AVERAGE_OF_VECTOR_ELEMENTS_H_

#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
double getParallelAverage(std::vector<int> vec);
int getSequentialSum(std::vector<int> vec);

#endif  // TASKS_TASK_1_KOSTIN_A_AVERAGE_OF_VECTOR_ELEMENTS_AVERAGE_OF_VECTOR_ELEMENTS_H_
