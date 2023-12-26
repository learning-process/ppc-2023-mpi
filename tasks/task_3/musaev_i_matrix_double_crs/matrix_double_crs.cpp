// Copyright 2023 Musaev Ilgar

#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "task_3/musaev_i_matrix_double_crs/matrix_double_crs.h"

bool isSrandCalled = false;

void InitializeMatrix(int N, int NZ, crsMatrix* mtx) {
    mtx->N = N;
    mtx->NZ = NZ;
    mtx->Value = new double[NZ];
    mtx->Col = new int[NZ];
    mtx->RowIndex = new int[N + 1];
}

void FreeMatrix(crsMatrix* mtx) {
    delete[] mtx->Value;
    delete[] mtx->Col;
    delete[] mtx->RowIndex;
}

void GenerateRegularCRS(int N, int cntInRow, crsMatrix* mtx) {
    std::mt19937 mersenne;
    mersenne.seed(static_cast<unsigned int>(time(0)));
    int i, j, k, f, tmp, notNull, c;
    notNull = cntInRow * N;
    InitializeMatrix(N, notNull, mtx);
    for (i = 0; i < N; i++) {
        for (j = 0; j < cntInRow; j++) {
            do {
                mtx->Col[i * cntInRow + j] = mersenne() % N;
                f = 0;
                for (k = 0; k < j; k++)
                    if (mtx->Col[i * cntInRow + j] ==
                        mtx->Col[i * cntInRow + k])
                        f = 1;
            } while (f == 1);
        }
        for (j = 0; j < cntInRow - 1; j++)
            for (k = 0; k < cntInRow - 1; k++)
                if (mtx->Col[i * cntInRow + k] > mtx->Col[i * cntInRow + k + 1]) {
                    tmp = mtx->Col[i * cntInRow + k];
                    mtx->Col[i * cntInRow + k] =
                        mtx->Col[i * cntInRow + k + 1];
                    mtx->Col[i * cntInRow + k + 1] = tmp;
                }
    }
    for (i = 0; i < cntInRow * N; i++)
        mtx->Value[i] = (static_cast<double>(mersenne() % 1000)/100);
    c = 0;
    for (i = 0; i <= N; i++) {
        mtx->RowIndex[i] = c;
        c += cntInRow;
    }
}

void MultiplicateGustafson(crsMatrix A, crsMatrix B, crsMatrix* C) {
    int strok_peredali_is_mtrA = A.N;
    std::vector<double> polnaya_stroka_C(A.N, 0);
    std::vector<double> crs_C_value_full;
    std::vector<int> crs_C_col_full;
    std::vector<int> crs_C_row_index_full;
    crs_C_row_index_full.push_back(0);
    for (int i = 0; i < strok_peredali_is_mtrA; i++) {
        for (int j = A.RowIndex[i]; j < A.RowIndex[i + 1]; j++) {
            int Col_elem_A = A.Col[j];
            for (int k = B.RowIndex[Col_elem_A]; k < B.RowIndex[Col_elem_A + 1]; k++) {
                polnaya_stroka_C[B.Col[k]] += A.Value[j] * B.Value[k];
            }
        }
        for (int k = 0; k < A.N; k++) {
            if (polnaya_stroka_C[k] != 0) {
                crs_C_value_full.push_back(polnaya_stroka_C[k]);
                crs_C_col_full.push_back(k);
                polnaya_stroka_C[k] = 0;
            }
        }
        crs_C_row_index_full.push_back(crs_C_value_full.size());
    }
    int row_index_size = crs_C_row_index_full.size();
    C->N = A.N;
    int add_elem_vsego = static_cast<double>(crs_C_value_full.size());
    C->NZ = add_elem_vsego;
    C->Col = new int[add_elem_vsego];
    C->Value = new double[add_elem_vsego];
    C->RowIndex = new int[row_index_size];
    for (int i = 0; i < add_elem_vsego; i++) {
        C->Col[i] = crs_C_col_full.at(i);
        C->Value[i] = crs_C_value_full.at(i);
    }
    for (int i = 0; i < row_index_size; i++) {
        C->RowIndex[i] = crs_C_row_index_full.at(i);
    }
}

int MultiplicateMPI(crsMatrix* A, crsMatrix* B, crsMatrix* C) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size == 1) {
        MultiplicateGustafson(*A, *B, C);
        return 0;
    }
    int* upakovka;
    upakovka = new int[3];
    int row_on_proc, ostatok_last_proc, vsego_elem_mtr_B;
    if (rank == 0) {
        C->N = A->N;
        row_on_proc = A->N / size;
        ostatok_last_proc = A->N % size;
        vsego_elem_mtr_B = B->RowIndex[B->N];
        upakovka[0] = A->N;
        upakovka[1] = vsego_elem_mtr_B;
        upakovka[2] = row_on_proc;
    }
    MPI_Bcast(&upakovka[0], 3, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        row_on_proc = upakovka[2];
        vsego_elem_mtr_B = upakovka[1];
        int N = upakovka[0];
        A->N = N;
        B->N = N;
        int size_row_index = N+1;
        A->RowIndex = new int[row_on_proc+1];
        B->Col = new int[vsego_elem_mtr_B];
        B->Value = new double[vsego_elem_mtr_B];
        B->RowIndex = new int[size_row_index];
    }
    MPI_Bcast(B->Col, vsego_elem_mtr_B, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B->Value, vsego_elem_mtr_B, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B->RowIndex, B->N+1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int otsilka = A->RowIndex[row_on_proc * i + ostatok_last_proc];
            MPI_Send(&A->RowIndex[row_on_proc * i + ostatok_last_proc], row_on_proc + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            int kol = A->RowIndex[row_on_proc*(i+1)+ostatok_last_proc]-A->RowIndex[row_on_proc*i+ostatok_last_proc];
            MPI_Send(&A->Col[otsilka], kol, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&A->Value[otsilka], kol, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
        }
        crsMatrix c;
        crsMatrix* C_temp = &c;
        create_part_crs_C(row_on_proc + ostatok_last_proc, A, B, C_temp);
        int* massiv_elem_proc_helperov;
        massiv_elem_proc_helperov = new int[size];
        massiv_elem_proc_helperov[0] = C_temp->RowIndex[ostatok_last_proc + row_on_proc - 1];
        MPI_Status st;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&massiv_elem_proc_helperov[i], 1, MPI_INT, i, 5, MPI_COMM_WORLD, &st);
        }
        int vsego_elem_so_vseh_proc = 0;
        for (int i = 0; i < size; i++) {
            vsego_elem_so_vseh_proc += massiv_elem_proc_helperov[i];
        }
        C->RowIndex = new int[A->N + 1];
        C->Col = new int[vsego_elem_so_vseh_proc];
        C->Value = new double[vsego_elem_so_vseh_proc];
        C->RowIndex[0] = 0;
        for (int i = 0; i < massiv_elem_proc_helperov[0]; i++) {
            C->Col[i] = C_temp->Col[i];
            C->Value[i] = C_temp->Value[i];
        }
        for (int i = 1; i <= ostatok_last_proc + row_on_proc; i++) {
            C->RowIndex[i] = C_temp->RowIndex[i-1];
        }
        MPI_Status s1, s2, s3;
        for (int i = 1; i < size; i++) {
            int chto = row_on_proc * i + ostatok_last_proc + 1;
            MPI_Recv(&C->RowIndex[chto], row_on_proc, MPI_INT, i, 1, MPI_COMM_WORLD, &s1);
            for (int j = 0; j < row_on_proc; j++) {
                C->RowIndex[chto +j] += C->RowIndex[row_on_proc * i + ostatok_last_proc];
            }
            int help = massiv_elem_proc_helperov[i];
            int index_prinytiya = C->RowIndex[row_on_proc * i + ostatok_last_proc];
            MPI_Recv(&C->Col[index_prinytiya], help, MPI_INT, i, 2, MPI_COMM_WORLD, &s2);
            MPI_Recv(&C->Value[index_prinytiya], help, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &s3);
        }
        FreeMatrix(C_temp);
        delete[] massiv_elem_proc_helperov;
    } else {
        MPI_Recv(&A->RowIndex[0], row_on_proc + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int sdvig_strok_k_starty = A->RowIndex[0];
        for (int i = 0; i < row_on_proc + 1; i++) {
            A->RowIndex[i] -= sdvig_strok_k_starty;
        }
        A->Col = new int[A->RowIndex[row_on_proc]];
        A->Value = new double[A->RowIndex[row_on_proc]];
        MPI_Status status1, status2;
        MPI_Recv(&A->Col[0], A->RowIndex[row_on_proc], MPI_INT, 0, 1, MPI_COMM_WORLD, &status1);
        MPI_Recv(&A->Value[0], A->RowIndex[row_on_proc], MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status2);
        create_part_crs_C(row_on_proc, A, B, C);
        int sozdali_elem = C->RowIndex[row_on_proc - 1];
        MPI_Send(&sozdali_elem, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
        MPI_Send(&C->RowIndex[0], row_on_proc, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&C->Col[0], sozdali_elem, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&C->Value[0], sozdali_elem, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        FreeMatrix(A);
        FreeMatrix(B);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != 0) {
        FreeMatrix(C);
    }
    return 0;
}

void create_part_crs_C(int row_peredali, crsMatrix* A, crsMatrix* B, crsMatrix* C) {
    int strok_peredali_is_mtrA = row_peredali;
    std::vector<double> polnaya_stroka_C(A->N, 0);
    std::vector<double> crs_C_value_full;
    std::vector<int> crs_C_col_full;
    std::vector<int> crs_C_row_index_full;
    for (int i = 0; i < strok_peredali_is_mtrA; i++) {
        for (int j = A->RowIndex[i]; j < A->RowIndex[i + 1]; j++) {
            int Col_elem_A = A->Col[j];
            for (int k = B->RowIndex[Col_elem_A]; k < B->RowIndex[Col_elem_A + 1]; k++) {
                polnaya_stroka_C[B->Col[k]] += A->Value[j] * B->Value[k];
            }
        }
        for (int k = 0; k < A->N; k++) {
            if (polnaya_stroka_C[k] != 0) {
                crs_C_value_full.push_back(polnaya_stroka_C[k]);
                crs_C_col_full.push_back(k);
                polnaya_stroka_C[k] = 0;
            }
        }
        crs_C_row_index_full.push_back(crs_C_value_full.size());
    }
    int row_index_size = crs_C_row_index_full.size();
    C->N = A->N;
    int add_elem_vsego = crs_C_value_full.size();
    C->Col = new int[add_elem_vsego];
    C->Value = new double[add_elem_vsego];
    C->RowIndex = new int[row_index_size];
    for (int i = 0; i < add_elem_vsego; i++) {
        C->Col[i] = crs_C_col_full.at(i);
        C->Value[i] = crs_C_value_full.at(i);
    }
    for (int i = 0; i < row_index_size; i++) {
        C->RowIndex[i] = crs_C_row_index_full.at(i);
    }
}

double** create_norm_mtr(crsMatrix A) {
    double** norm_mtr;
    norm_mtr = new double*[A.N];
    for (int j = 0; j < A.N; j++) {
        norm_mtr[j] = new double[A.N];
    }
    for (int j = 0; j < A.N; j++) {
        for (int i = 0; i < A.N; i++) {
            norm_mtr[i][j] = 0;
        }
    }
    for (int i = 0; i < A.N; i++) {
        for (int j = A.RowIndex[i]; j < A.RowIndex[i + 1]; j++) {
            norm_mtr[i][A.Col[j]] = A.Value[j];
        }
    }
    return norm_mtr;
}

double** mult_norm_matr(double** A, double** B, int N) {
    double** C;
    C = new double* [N];
    for (int j = 0; j < N; j++) {
        C[j] = new double[N];
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
        std::cout << std::endl;
    }
    return C;
}
