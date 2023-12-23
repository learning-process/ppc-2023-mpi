// Copyright 2023 Borovkov Sergey
#ifndef TASKS_TASK_3_BOROVKOV_S_GAUSS_VERT_ALG_GAUSS_VERT_H_
#define TASKS_TASK_3_BOROVKOV_S_GAUSS_VERT_ALG_GAUSS_VERT_H_
int* getRandomImage(int width, int height);
int* getSequentialGauss(const int* image, int width, int height);
int* getParallelGauss(const int* image, int width, int height);

#endif  // TASKS_TASK_3_BOROVKOV_S_GAUSS_VERT_ALG_GAUSS_VERT_H_
