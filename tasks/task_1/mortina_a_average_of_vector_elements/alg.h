// Copyright 2023 Mortina Anastasiya
#ifndef TASKS_TASK_1_MORTINA_A_AVERAGE_OF_VECTOR_ELEMENTS_ALG_H_
#define TASKS_TASK_1_MORTINA_A_AVERAGE_OF_VECTOR_ELEMENTS_ALG_H_

#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
#include <random>


int generateRandomNumbers(int min, int max);
std::vector<int> getRandomVector(int size);
double midValueOfVectorParallel(const std::vector<int>& vector);
double sumOfVectorSequential(const std::vector<int>& vector);

#endif  // TASKS_TASK_1_MORTINA_A_AVERAGE_OF_VECTOR_ELEMENTS_ALG_H_
