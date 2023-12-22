// Copyright 2023 Sadikov Damir
#ifndef TASKS_TASK_1_SADIKOV_D_DOT_PRODUCT_DOT_PRODUCT_H_
#define TASKS_TASK_1_SADIKOV_D_DOT_PRODUCT_DOT_PRODUCT_H_

#include <vector>

std::vector<int> getRandomVector(int sz);
int getSequentialDotProduct(const std::vector<int>& A, const std::vector<int>& B);
int getParallelDotProduct(const std::vector<int>& A,
                          const std::vector<int>& B, int vector_size);

#endif  // TASKS_TASK_1_SADIKOV_D_DOT_PRODUCT_DOT_PRODUCT_H_
