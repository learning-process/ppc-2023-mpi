// Copyright 2023 Mironov Ilya
#include <mpi.h>

#include <vector>

#include "task_3/mironov_i_trapezoid_method/trapezoid_method.h"


double plus(double x, double y) { return x + y; }
double minus(double x, double y) { return x - y; }
double multiple(double x, double y) { return x * y; }
double division(double x, double y) { return x / y; }
double quadSum(double x, double y) { return (x * x + y * y); }

double sequentialTrapezoidalRule(double xa, double xb, double ya, double yb, int n, double (*func)(double, double)) {
    int count = n * n;
    double xh = (xb - xa) / n;
    double yh = (yb - ya) / n;
    double result = .0;
    for (int i = 0; i < count; ++i) {
        double xk = xa + xh * (i % n + 0.5);
        double yk = ya + yh * (i % n + 0.5);
        double fk = func(xk, yk);
        result += fk;
    }

    return result * xh * yh;
}

double parallelTrapezoidalRule(double xa, double xb, double ya, double yb, int n, double (*func)(double, double)) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return sequentialTrapezoidalRule(xa, xb, ya, yb, n, func);
    }

    double xh = (xb - xa) / n;
    double yh = (yb - ya) / n;
    double result = .0;

    int delta = n * n / size;
    int start = rank * delta;
    int end = start + delta;

    for (int i = start; i < end; ++i) {
        double xk = xa + xh * (i % n + 0.5);
        double yk = ya + yh * (i % n + 0.5);
        double fk = func(xk, yk);
        result += fk;
    }

    double globalResult;

    MPI_Reduce(&result, &globalResult, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return globalResult * xh * yh;
}
