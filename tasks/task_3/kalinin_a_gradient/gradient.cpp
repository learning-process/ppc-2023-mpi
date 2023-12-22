// Copyright 2023 Kalinin Alexandr
#include "task_1/kalinin_a_gradient/gradient.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution < int > dist(0, 10);

Vector RandVec(int n) {
    Vector V(n);
    for (int i = 0; i < n; ++i) {
        V[i] = dist(gen) % 3;
    }
    return V;
}
Vector RandMat(int n) {
    Vector M(n * n);
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            M[i * n + j] = dist(gen) % 3;
            M[j * n + i] = dist(gen) % 3;
        }
    }
    return M;
}

double scalar_mult(const Vector& x,
    const Vector& y) {
    double res = 0;
    for (int i = 0; i < x.size(); i++) {
        res = res + x[i] * y[i];
    }
    return res;
}
