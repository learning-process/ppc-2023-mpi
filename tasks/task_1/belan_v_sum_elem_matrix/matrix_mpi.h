/* Copyright 2023 Belan Vadim */

#ifndef TASKS_TASK_1_BELAN_V_SUM_ELEM_MATRIX_MATRIX_MPI_H_
#define TASKS_TASK_1_BELAN_V_SUM_ELEM_MATRIX_MATRIX_MPI_H_

int calcMatrixSum(int** matrix, int size, int world_rank, int world_size);
void deleteMatrix(int** matrix, int size);

#endif  // TASKS_TASK_1_BELAN_V_SUM_ELEM_MATRIX_MATRIX_MPI_H_
