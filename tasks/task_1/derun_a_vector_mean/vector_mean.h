// Copyright 2023 Derun Andrew
#ifndef TASKS_TASK_1_DERUN_A_VECTOR_MEAN_VECTOR_MEAN_H_
#define TASKS_TASK_1_DERUN_A_VECTOR_MEAN_VECTOR_MEAN_H_
#include <algorithm>
#include <random>
#include <vector>

template <class T>
std::vector<T> getRandomVector(int size, int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(min, max);

  if (size < 0) {
    size = 0;
  }
  std::vector<T> vec(size);
  for (int i = 0; i < size; i++) {
    vec[i] = dist(gen);
  }
  return vec;
}
float getParallelMean(std::vector<int> parall_vec, int size);
float getSequentialMean(std::vector<int> sequent_vec);

#endif  // TASKS_TASK_1_DERUN_A_VECTOR_MEAN_VECTOR_MEAN_H_
