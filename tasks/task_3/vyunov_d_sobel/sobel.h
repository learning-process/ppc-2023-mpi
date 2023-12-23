// Copyright 2023 Vyunov Danila
#ifndef TASKS_TASK_3_VYUNOV_D_SOBEL_SOBEL_H_
#define TASKS_TASK_3_VYUNOV_D_SOBEL_SOBEL_H_

const int kernelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
const int kernelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

int* getRandomImage(int width, int height);
int* getSequentialSobel(int* image, int width, int height);
int* getParallelSobel(int* image, int width, int height);

#endif  // TASKS_TASK_3_VYUNOV_D_SOBEL_SOBEL_H_
