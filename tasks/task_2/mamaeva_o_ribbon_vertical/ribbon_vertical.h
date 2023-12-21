// Copyright 2023 Mamaeva Olga
#ifndef TASKS_TASK_2_MAMAEVA_O_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
#define TASKS_TASK_2_MAMAEVA_O_RIBBON_VERTICAL_RIBBON_VERTICAL_H_

int* initEmptyMatrix(int rows);
int* getRandomMatrix(int rows, int cols);
void SequentialMatrixMultiplication(const int* A, const int ARows,
                                    const int ACols, const int* B,
                                    const int BRows, int* C);
void ParallelMatrixMultiplication(const int* A, const int ARows,
                                  const int ACols, const int* B,
                                  const int BRows, int* C);

#endif  // TASKS_TASK_2_MAMAEVA_O_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
