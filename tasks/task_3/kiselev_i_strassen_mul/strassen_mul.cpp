// Copyright 2023 Kiselev Igor
#include "task_3/kiselev_i_strassen_mul/strassen_mul.h"

std::vector<std::vector<double>> matrix_addition(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B) {
    int n = A.size();
    std::vector<std::vector<double>> C(n, std::vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}

std::vector<std::vector<double>> matrix_subtraction(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B) {
    int n = A.size();
    std::vector<std::vector<double>> C(n, std::vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }

    return C;
}

std::vector<std::vector<double>> matrix_multiplication(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B) {
    int n = A.size();
    std::vector<std::vector<double>> C(n, std::vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}





void subdivide_matrix(const std::vector<std::vector<double>>& A,
                      std::vector<std::vector<double>>& A11,
                      std::vector<std::vector<double>>& A12,
                      std::vector<std::vector<double>>& A21,
                      std::vector<std::vector<double>>& A22) {
    int n = A.size();
    int m = n / 2;

    A11.resize(m, std::vector<double>(m));
    A12.resize(m, std::vector<double>(m));
    A21.resize(m, std::vector<double>(m));
    A22.resize(m, std::vector<double>(m));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + m];
            A21[i][j] = A[i + m][j];
            A22[i][j] = A[i + m][j + m];
        }
    }
}

std::vector<std::vector<double>> merge_matrix(const std::vector<std::vector<double>>& C11,
                                              const std::vector<std::vector<double>>& C12,
                                              const std::vector<std::vector<double>>& C21,
                                              const std::vector<std::vector<double>>& C22) {
    int n = C11.size();
    int m = 2 * n;

    std::vector<std::vector<double>> C(m, std::vector<double>(m));

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

std::vector<std::vector<double>> strassen_mult(const std::vector<std::vector<double>>& A,
                                               const std::vector<std::vector<double>>& B) {
    int n = A.size();

    if (n == 1) {
        std::vector<std::vector<double>> C(1, std::vector<double>(1));
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    std::vector<std::vector<double>> C(n, std::vector<double>(n));

    std::vector<std::vector<double>> A11, A12, A21, A22;
    std::vector<std::vector<double>> B11, B12, B21, B22;
    subdivide_matrix(A, A11, A12, A21, A22);
    subdivide_matrix(B, B11, B12, B21, B22);

    std::vector<std::vector<double>> P1 = strassen_mult(matrix_addition(A11, A22), matrix_addition(B11, B22));
    std::vector<std::vector<double>> P2 = strassen_mult(matrix_addition(A21, A22), B11);
    std::vector<std::vector<double>> P3 = strassen_mult(A11, matrix_subtraction(B12, B22));
    std::vector<std::vector<double>> P4 = strassen_mult(A22, matrix_subtraction(B21, B11));
    std::vector<std::vector<double>> P5 = strassen_mult(matrix_addition(A11, A12), B22);
    std::vector<std::vector<double>> P6 = strassen_mult(matrix_subtraction(A21, A11), matrix_addition(B11, B12));
    std::vector<std::vector<double>> P7 = strassen_mult(matrix_subtraction(A12, A22), matrix_addition(B21, B22));

    A11.clear(); A12.clear(); A21.clear(); A22.clear();
    B11.clear(); B12.clear(); B21.clear(); B22.clear();

    std::vector<std::vector<double>> C11 = matrix_addition(matrix_subtraction(matrix_addition(P1, P4), P5), P7);
    std::vector<std::vector<double>> C12 = matrix_addition(P3, P5);
    std::vector<std::vector<double>> C21 = matrix_addition(P2, P4);
    std::vector<std::vector<double>> C22 = matrix_subtraction(matrix_subtraction(matrix_addition(P1, P3), P2), P6);

    P1.clear(); P2.clear(); P3.clear(); P4.clear(); P5.clear(); P6.clear() ;P7.clear();
    C = merge_matrix(C11, C12, C21, C22);

    C11.clear(); C12.clear(); C21.clear(); C22.clear();
    return C;
}
// kiselev_i
