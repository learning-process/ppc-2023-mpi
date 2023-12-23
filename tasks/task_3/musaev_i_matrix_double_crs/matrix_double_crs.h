// Copyright 2023 Musaev Ilgar
#ifndef TASKS_TASK_3_MUSAEV_I_MATRIX_DOUBLE_CRS_MATRIX_DOUBLE_CRS_H_
#define TASKS_TASK_3_MUSAEV_I_MATRIX_DOUBLE_CRS_MATRIX_DOUBLE_CRS_H_

#include <vector>

struct crsMatrix {
    int N;
    int NZ;
    double* Value;
    int* Col;
    int* RowIndex;
};
void InitializeMatrix(int N, int NZ, crsMatrix* mtx);
void FreeMatrix(crsMatrix* mtx);
double** mult_norm_matr(double** A, double** B, int N);
double** create_norm_mtr(crsMatrix A);
int MultiplicateMPI(crsMatrix* A, crsMatrix* B, crsMatrix* C);
void create_part_crs_C(int row_peredali, crsMatrix* A, crsMatrix* B, crsMatrix* C);
void MultiplicateGustafson(crsMatrix A, crsMatrix B, crsMatrix* C);
void GenerateRegularCRS(int N, int cntInRow, crsMatrix* mtx);


#endif  // TASKS_TASK_3_MUSAEV_I_MATRIX_DOUBLE_CRS_MATRIX_DOUBLE_CRS_H_
