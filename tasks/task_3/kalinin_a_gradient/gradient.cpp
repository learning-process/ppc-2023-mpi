// Copyright 2023 Kalinin Alexandr
#include "task_3/kalinin_a_gradient/gradient.h"

Vector RandVec(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    Vector V(n);
    for (int i = 0; i < n; ++i) {
        V[i] = dist(gen) % 3;
    }
    return V;
}

Vector RandMat(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    Vector M(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            M[i * n + j] = dist(gen) % 3;
            M[j * n + i] = dist(gen) % 3;
        }
    }
    return M;
}

double scalar_mult(const Vector& x, const Vector& y) {
    double res = 0;
    for (int i = 0; i < x.size(); i++) {
        res = res + x[i] * y[i];
    }
    return res;
}

Vector mult_MxV(const Vector& M,
    const Vector& V) {
    Vector res(M.size() / V.size());
    for (int i = 0; i < M.size() / V.size(); i++) {
        res[i] = 0;
        for (int j = 0; j < V.size(); j++) {
            res[i] += M[i * V.size() + j] * V[j];
        }
    }
    return res;
}

Vector Serial_method_gradient(const Vector& matrixA,
    const Vector& vectorB, int size) {
    Vector gradient(size), direction(size), temp, solution(size);
    double errorThreshold = 0.01, alpha = 0.0, beta = 0.0;
    for (int i = 0; i < size; i++) {
        solution[i] = 1;
    }
    temp = mult_MxV(matrixA, solution);
    for (int i = 0; i < size; i++) {
        gradient[i] = vectorB[i] - temp[i];
        direction[i] = gradient[i];
    }

    int iteration = 0;
    do {
        temp = mult_MxV(matrixA, direction);
        double numerator = scalar_mult(gradient, gradient);
        alpha = numerator / scalar_mult(direction, temp);
        for (int i = 0; i < size; i++) {
            solution[i] = solution[i] + alpha * direction[i];
            gradient[i] = gradient[i] - alpha * temp[i];
        }
        beta = scalar_mult(gradient, gradient) / numerator;
        for (int i = 0; i < size; i++) {
            direction[i] = gradient[i] + beta * direction[i];
        }
        iteration++;
    } while (sqrt(scalar_mult(gradient, gradient))
        > errorThreshold && iteration <= size);
    return solution;
}

