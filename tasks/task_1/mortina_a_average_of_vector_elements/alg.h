// Copyright 2023 Mortina Anastasia
#ifndef TASKS_TASK_1_MORTINA_A_AVERAGE_OF_VECTOR_ELEMENTS_ALG_H_
#define TASKS_TASK_1_MORTINA_A_AVERAGE_OF_VECTOR_ELEMENTS_ALG_H_

#include <vector>
#include <random>
#include <functional>
#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>

int generateRandomNumbers(int min, int max);
std::vector<int> getRandomVector(int size);
double midValueOfVectorParallel(const std::vector<int>& vector);
double sumOfVectorSequential(const std::vector<int>& vector);

#endif  // TASKS_TASK_1_MORTINA_A_AVERAGE_OF_VECTOR_ELEMENTS_ALG_H_
