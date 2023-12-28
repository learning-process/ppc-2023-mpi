// Copyright 2023 Kiselev Igor
#include "task_3/kiselev_i_strassen_mul_double/strassen_mul.h"

std::vector<std::vector<double>> sumMatrix(const std::vector<std::vector<double>> &sumA,
    const std::vector<std::vector<double>> &sumB, int size, bool sign) {
    std::vector<std::vector<double>> resMatrix(size, std::vector<double>(size, 0));

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            resMatrix[i][j] = sign ? sumA[i][j] + sumB[i][j] : sumA[i][j] - sumB[i][j];

    return resMatrix;
}

std::vector<std::vector<double>> subMatrix(const std::vector<std::vector<double>> &original,
    int subi, int subj, int size) {

    int newSize = size / 2;
    std::vector<std::vector<double>> subMatrix(newSize, std::vector<double>(newSize, 0));

    for (int j = 0; j < newSize; j++)
        for (int i = 0; i < newSize; i++)
            subMatrix[i][j] = original[i + (subi - 1) * newSize][j + (subj - 1) * newSize];
    return subMatrix;
}

std::vector<std::vector<double>> collectMatrix(const std::vector<std::vector<double>> &quarter1,
    const std::vector<std::vector<double>> &quarter2, const std::vector<std::vector<double>> &quarter3,
    const std::vector<std::vector<double>> &quarter4, int size) {

    int newSize = size / 2;
    std::vector<std::vector<double>> colMatrix(size, std::vector<double>(size, 0));

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            colMatrix[i][j] = quarter1[i][j];
            colMatrix[i][j + newSize] = quarter3[i][j];
            colMatrix[i + newSize][j] = quarter2[i][j];
            colMatrix[i + newSize][j + newSize] = quarter4[i][j];
        }
    }

    return colMatrix;
}

std::vector<std::vector<double>> SeqMulMatrix(const std::vector<std::vector<double>> &SeqA,
    const std::vector<std::vector<double>> &SeqB, int n) {

    std::vector<std::vector<double>> resMatrix(n, std::vector<double>(n, 0));

    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++)
                resMatrix[j][i] += SeqA[j][k] * SeqB[k][i];

    return resMatrix;
}

std::vector<std::vector<double>> Strassen(const std::vector<std::vector<double>> &StrA,
    const std::vector<std::vector<double>> &StrB, int size) {

    std::vector<std::vector<double>> strMatrix(size, std::vector<double>(size, 0));

    if (size == 1) {
        strMatrix[0][0] = StrA[0][0] * StrB[0][0];
        return strMatrix;
    }

    int newSize = size / 2;
    std::vector<double> inner(newSize);
    std::vector<std::vector<double>>
        A11(newSize, inner), A12(newSize, inner), A21(newSize, inner), A22(newSize, inner),
        B11(newSize, inner), B12(newSize, inner), B21(newSize, inner), B22(newSize, inner),
        C11(newSize, inner), C12(newSize, inner), C21(newSize, inner), C22(newSize, inner),
        M1(newSize, inner), M2(newSize, inner), M3(newSize, inner), M4(newSize, inner),
        M5(newSize, inner), M6(newSize, inner), M7(newSize, inner);

    A11 = subMatrix(StrA, 1, 1, size);
    A12 = subMatrix(StrA, 1, 2, size);
    A21 = subMatrix(StrA, 2, 1, size);
    A22 = subMatrix(StrA, 2, 2, size);
    B11 = subMatrix(StrB, 1, 1, size);
    B12 = subMatrix(StrB, 1, 2, size);
    B21 = subMatrix(StrB, 2, 1, size);
    B22 = subMatrix(StrB, 2, 2, size);

    M1 = Strassen(sumMatrix(A11, A22, newSize, true), sumMatrix(B11, B22, newSize, true), newSize);
    M2 = Strassen(sumMatrix(A21, A22, newSize, true), B11, newSize);
    M3 = Strassen(A11, sumMatrix(B12, B22, newSize, false), newSize);
    M4 = Strassen(A22, sumMatrix(B21, B11, newSize, false), newSize);
    M5 = Strassen(sumMatrix(A11, A12, newSize, true), B22, newSize);
    M6 = Strassen(sumMatrix(A21, A11, newSize, false), sumMatrix(B11, B12, newSize, true), newSize);
    M7 = Strassen(sumMatrix(A12, A22, newSize, false), sumMatrix(B21, B22, newSize, true), newSize);

    C11 = sumMatrix(sumMatrix(M1, M4, newSize, true), sumMatrix(M7, M5, newSize, false), newSize, true);
    C12 = sumMatrix(M3, M5, newSize, true);
    C21 = sumMatrix(M2, M4, newSize, true);
    C22 = sumMatrix(sumMatrix(M1, M2, newSize, false), sumMatrix(M3, M6, newSize, true), newSize, true);

    strMatrix = collectMatrix(C11, C21, C12, C22, size);

    return strMatrix;
}
// kiselev_i
