// Copyright 2023 Sredneva Anastasiya
#ifndef TASKS_TASK_3_SREDNEVA_A_SOBEL_OPERATOR_SOBEL_OPERATOR_H_
#define TASKS_TASK_3_SREDNEVA_A_SOBEL_OPERATOR_SOBEL_OPERATOR_H_

#include <mpi.h>
#include <vector>

int getSobelX(int i);
int getSobelY(int i);
std::vector<uint8_t> getRandomPicture(int n, int m, uint8_t l, uint8_t r);
std::vector<uint8_t> getParallelSobel(const std::vector<uint8_t>& pict, int n, int m);
std::vector<uint8_t> getSequentialSobel(const std::vector<uint8_t>& pict, int n, int m);


#endif  // TASKS_TASK_3_SREDNEVA_A_SOBEL_OPERATOR_SOBEL_OPERATOR_H_
