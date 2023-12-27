// Copyright 2023 Kiselev Igor
#include "task_1/kiselev_i_strassen_mul/strassen_mul.h"

vector<vector<double>> matrix_addition(const vector<vector<double>> &A, const vector<vector<double>> &B) {

    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}

vector<vector<double>> matrix_subtraction(const vector<vector<double>>& A, const vector<vector<double>>& B) {

    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }

    return C;
}

// Функция для умножения матриц
vector<vector<double>> matrix_multiplication(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// Функция для деления матрицы на 4 подматрицы
void subdivide_matrix(const vector<vector<double>>& A,
                      vector<vector<double>>& A11,
                      vector<vector<double>>& A12,
                      vector<vector<double>>& A21,
                      vector<vector<double>>& A22) {
    int n = A.size();
    int m = n / 2;

    A11.resize(m, vector<double>(m));
    A12.resize(m, vector<double>(m));
    A21.resize(m, vector<double>(m));
    A22.resize(m, vector<double>(m));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + m];
            A21[i][j] = A[i + m][j];
            A22[i][j] = A[i + m][j + m];
        }
    }
}

vector<vector<double>> merge_matrix(const vector<vector<double>>& C11,
                                              const vector<vector<double>>& C12,
                                              const vector<vector<double>>& C21,
                                              const vector<vector<double>>& C22) {
    int n = C11.size();
    int m = 2 * n;

    vector<vector<double>> C(m, vector<double>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = C11[i][j];
            C[i][j + n] = C12[i][j];
            C[i + n][j] = C21[i][j];
            C[i + n][j + n] = C22[i][j];
        }
    }

    return C;
}

vector<vector<double>> strassen_mult(const vector<vector<double>>& A,
                                               const vector<vector<double>>& B) {
    int n = A.size();

    if (n == 1) {
        vector<vector<double>> C(1, vector<double>(1));
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    vector<vector<double>> C(n, vector<double>(n));

    vector<vector<double>> A11, A12, A21, A22;
    vector<vector<double>> B11, B12, B21, B22;
    subdivide_matrix(A, A11, A12, A21, A22);
    subdivide_matrix(B, B11, B12, B21, B22);

    vector<vector<double>> P1 = strassen_mult(matrix_addition(A11, A22), matrix_addition(B11, B22));
    vector<vector<double>> P2 = strassen_mult(matrix_addition(A21, A22), B11);
    vector<vector<double>> P3 = strassen_mult(A11, matrix_subtraction(B12, B22));
    vector<vector<double>> P4 = strassen_mult(A22, matrix_subtraction(B21, B11));
    vector<vector<double>> P5 = strassen_mult(matrix_addition(A11, A12), B22);
    vector<vector<double>> P6 = strassen_mult(matrix_subtraction(A21, A11), matrix_addition(B11, B12));
    vector<vector<double>> P7 = strassen_mult(matrix_subtraction(A12, A22), matrix_addition(B21, B22));

    A11.clear();A12.clear();A21.clear();A22.clear();
    B11.clear();B12.clear();B21.clear();B22.clear();

    vector<vector<double>> C11 = matrix_addition(matrix_subtraction(matrix_addition(P1, P4), P5), P7);
    vector<vector<double>> C12 = matrix_addition(P3, P5);
    vector<vector<double>> C21 = matrix_addition(P2, P4);
    vector<vector<double>> C22 = matrix_subtraction(matrix_subtraction(matrix_addition(P1, P3), P2), P6);

    P1.clear();P2.clear();P3.clear();P4.clear();P5.clear();P6.clear();P7.clear();
    C = merge_matrix(C11, C12, C21, C22);

    C11.clear(); C12.clear(); C21.clear(); C22.clear();
    return C;
}
//kiselev_i
