// Copyright Zhatkins Vyacheslav

#ifndef TASKS_TASK_1_ZHATKIN_V_SCALAR_PRODUCT_SCALAR_PRODUCT_H_
#define TASKS_TASK_1_ZHATKIN_V_SCALAR_PRODUCT_SCALAR_PRODUCT_H_

#define MAX_NUMBER 200
#define MIN_NUMBER -100


#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>


std::vector<int> creatRandomVector(const int v_size);

int getSequentialScalarProduct(const std::vector<int>& firsVector,
 const std::vector<int>& secondVector);

int getParallelScalarProduct(const std::vector<int>& firsVector,
 const std::vector<int>& secondVector);

#endif  // TASKS_TASK_1_ZHATKIN_V_SCALAR_PRODUCT_SCALAR_PRODUCT_H_
