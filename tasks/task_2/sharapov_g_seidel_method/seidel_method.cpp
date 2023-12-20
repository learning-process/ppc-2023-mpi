// Copyright 2023 Sharapov Georgiy
#include <mpi.h>
#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>
#include "task_2/sharapov_g_seidel_method/seidel_method.h"

double* generateMatrix(size_t size) {
    std::random_device rd;
    std::mt19937 rng;
    rng.seed(rd());
    std::uniform_int_distribution<int> uint_dist(-200, 200);

    double* matrix = new double[size * (size + 1)];

    for (int i = 0; i < size; i++) {
        int sum = 0;
        for (int j = 0; j < size + 1; j++) {
            if (i == j) continue;
            matrix[i * (size + 1) + j] = uint_dist(rng) / 3;
            sum += abs(matrix[i * (size + 1) + j]);
        }
        matrix[i * (size + 1) + i] = sum + abs(uint_dist(rng));
    }

    return matrix;
}

double* seidelMethod(double* matrix, size_t n, double eps) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    //  declaration of variables (more than I would like...)
    int *ProcSizes = new int[ProcNum]();
    int *ProcDisp = new int[ProcNum]();
    for (int i = 0; i < ProcNum; i++) {
        ProcSizes[i] = n / ProcNum + (i < n % ProcNum ? 1 : 0);
        ProcDisp[i] = i * (n / ProcNum) + std::min<int>(i, n % ProcNum);
    }

    bool condition = false;
    bool local_condition = true;

    int prev_s = 0;
    int prev_e = n;
    int current_proc = 0;

    int rows_amount = ProcSizes[ProcRank];
    int start_row = ProcDisp[ProcRank];
    int end_row = ProcDisp[ProcRank] + ProcSizes[ProcRank];

    double add_x = 0.0;

    double *right_part = new double[rows_amount]();
    double *left_part = new double[rows_amount]();

    double *ans = new double[n]();
    double *local_ans = new double[n]();
    double *rows = new double[rows_amount * (n + 1)]();

    //  generating augmented matrix and sending parts of it
    //  to corresponding processors
    int *ProcElem = new int[ProcNum]();
    int *ProcElemDisp = new int[ProcNum]();
    for (int i = 0; i < ProcNum; i++) {
        ProcElem[i] = ProcSizes[i] * (n + 1);
        ProcElemDisp[i] = ProcDisp[i] * (n + 1);
    }

    if (ProcRank == 0) {
        MPI_Scatterv(matrix, ProcElem, ProcElemDisp, MPI_DOUBLE, rows,
                    rows_amount * (n + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatterv(nullptr, nullptr, nullptr, MPI_DOUBLE, rows, rows_amount * (n + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    //  devide all elements in matrix by elements on diagonal
    for (int row = 0; row < rows_amount; row++) {
        double devider = rows[(row * (n + 1)) + (row + start_row)];
        for (int element = 0; element < n + 1; element++)
            rows[row * (n + 1) + element] /= devider;
    }

    //  main loop
    while (!condition) {
        //  each full for loop go through one approximation step
        for (int i = 0; i < ProcNum; i++) {
            //  find left and right part for each row with new ans[] data
            for (int row = 0; row < rows_amount; row++) {
                if (prev_s >= end_row) {
                    for (int elem = prev_s; elem < prev_e; elem++) {
                        right_part[row] -= ans[elem] * rows[row * (n + 1) + elem];
                    }
                } else if (prev_e <= start_row) {
                    for (int elem = prev_s; elem < prev_e; elem++) {
                        left_part[row] -= ans[elem] * rows[row * (n + 1) + elem];
                    }
                } else {
                    break;
                }
            }

            //  find x valuse for current_proc and send it to other
            if (current_proc == ProcRank) {
                for (int row = 0; row < rows_amount; row++) {
                    //  add_x - values that are right under diagonal X`s and
                    //  weren`t included in left_part
                    add_x = 0.0;
                    for (int add_idx = 0; add_idx < row; add_idx++) {
                        add_x -= local_ans[add_idx] * rows[(row * (n + 1)) + (start_row + add_idx)];
                    }
                    local_ans[row] = left_part[row] + add_x + right_part[row] + rows[row * (n + 1) + n];
                    left_part[row] = 0.0;
                    right_part[row] = 0.0;
                }

                //  find right_part with new ans[] (local_ans[]) data
                for (int row = 0; row < rows_amount; row++) {
                    for (int elem = start_row + row + 1; elem < end_row; elem++) {
                        right_part[row] -= local_ans[elem - start_row] * rows[row * (n + 1) + elem];
                    }
                }
                //  send founded X`s to all processors
                MPI_Bcast(local_ans, n, MPI_DOUBLE, current_proc, MPI_COMM_WORLD);
            } else {
                MPI_Bcast(local_ans, n, MPI_DOUBLE, current_proc, MPI_COMM_WORLD);
            }

            //  copy local X`s to answer
            for (int i = ProcDisp[current_proc]; i < ProcDisp[current_proc] + ProcSizes[current_proc]; i++) {
                ans[i] = local_ans[i - ProcDisp[current_proc]];
            }
            for (int elem = 0; elem < rows_amount; elem++) local_ans[elem] = 0.0;

            //  change info about latest processor that
            //  find it`s X`s
            prev_s = ProcDisp[current_proc];
            prev_e = prev_s + ProcSizes[current_proc];
            current_proc = (current_proc + 1) % ProcNum;
        }

        //  check for condition (Ax - B) < (eps)
        if (ProcRank == 0) {
            double tmp_value = 0.0;
            for (int row = 0; row < n; row++) {
                tmp_value = 0.0;
                for (int elem = 0; elem < n; elem++) {
                    tmp_value += ans[elem] * matrix[row * (n + 1) + elem];
                }
                if (fabs(matrix[row * (n + 1) + n] - tmp_value) > eps) {
                    condition = false;
                    break;
                }
                condition = true;
            }
        }

        MPI_Bcast(&condition, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
    }

    delete[] rows;
    delete[] ProcSizes;
    delete[] ProcDisp;
    delete[] local_ans;
    delete right_part;
    delete left_part;

    return ans;
}
