// Copyright 2023 Kashirin Alexander
#ifndef TASKS_TASK_3_KASHIRIN_A_CCS_MATRIX_MULT_CCS_MATRIX_MULT_H_
#define TASKS_TASK_3_KASHIRIN_A_CCS_MATRIX_MULT_CCS_MATRIX_MULT_H_

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

struct StructMatrix {
    int* Values = nullptr; int vals = 0;
    int* RowInd = nullptr; int rows = 0;
    int* ColPtr = nullptr; int cols = 0;
};

class ClassMatrix {
public:

    StructMatrix ccsMatrix;
    friend ClassMatrix MultiplyCCS(ClassMatrix* m1, ClassMatrix* m2);
    ClassMatrix() {}
    ClassMatrix(const int* matrix, int rows, int cols);
    explicit ClassMatrix(StructMatrix* props);
    ClassMatrix TransposeCCS() const;
    ClassMatrix& operator=(const ClassMatrix& sm);
    ClassMatrix& operator=(ClassMatrix&& sm);
    ClassMatrix operator*(const ClassMatrix& m) const;
    void clear();
    ~ClassMatrix();
};

ClassMatrix MultiplyCCS(ClassMatrix* m1, ClassMatrix* m2);

#endif  // TASKS_TASK_3_KASHIRIN_A_CCS_MATRIX_MULT_CCS_MATRIX_MULT_H_
