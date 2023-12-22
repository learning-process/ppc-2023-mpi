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

Vector Serial_method_gradient(const Vector& A,
    const Vector& b, int n) {
    Vector r0(n), p0(n), tmp, x(n);
    double E = 0.01, c1 = 0.0, c2 = 0.0;
    for (int i = 0; i < n; i++) {
        x[i] = 1;
    }
    tmp = mult_MxV(A, x);
    for (int i = 0; i < n; i++) {
        r0[i] = b[i] - tmp[i];
        p0[i] = r0[i];
    }

    int j = 0;
    do {
        tmp = mult_MxV(A, p0);
        double t = scalar_mult(r0, r0);
        c1 = t / scalar_mult(p0, tmp);
        for (int i = 0; i < n; i++) {
            x[i] = x[i] + c1 * p0[i];
            r0[i] = r0[i] - c1 * tmp[i];
        }
        c2 = scalar_mult(r0, r0) / t;
        for (int i = 0; i < n; i++) {
            p0[i] = r0[i] + c2 * p0[i];
        }
        j++;
    } while (sqrt(scalar_mult(r0, r0)) > E && j <= n);
    return x;
}