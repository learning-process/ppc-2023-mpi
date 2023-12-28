// Copyright 2023 Chuvashov Andrey

#ifndef TASKS_TASK_3_CHUVASHOV_A_THE_GRAHAM_PASSAGE_THE_GRAHAM_PASSAGE_H_
#define TASKS_TASK_3_CHUVASHOV_A_THE_GRAHAM_PASSAGE_THE_GRAHAM_PASSAGE_H_

#include <mpi.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <random>

class Point {
 public:
    double x;
    double y;
    Point();
    Point(double _x, double _y);
    Point& operator=(const Point& A) {
        if (this != &A) {
            x = A.x;
            y = A.y;
        }
        return *this;
    }
    bool operator==(const Point& A) {
        return ((x == A.x) && (y == A.y));
    }
    bool operator!=(const Point& A) {
        return ((x != A.x) && (y != A.y));
    }
    Point Min(const Point& A, const Point& B);
    ~Point();
};

class Vector {
 public:
    double X;
    double Y;
    Vector();
    Vector(Point A, Point B);
    Vector(double x, double y);
    ~Vector();
};

double Modul(const Vector& Vec);

double Scalar(const Vector& Vec1, const Vector& Vec2);

double Cosinus(const Vector& A, const Vector& B);

bool sideDef(double x1, double y1, double x2, double y2, double x3, double y3);

std::vector<Point> GrahamsPasParal(const std::vector<Point>& Points);

std::vector<Point> GrahamsPasOneProc(const std::vector<Point>& Points);

Point SearchMinPoint(const std::vector<Point>& Points);

std::vector<Point> SortPoints(const std::vector<Point>& Points);

bool pointsEqual(double x1, double y1, double x2, double y2);

std::vector<Point> GeneratePoints(int n);

#endif  // TASKS_TASK_3_CHUVASHOV_A_THE_GRAHAM_PASSAGE_THE_GRAHAM_PASSAGE_H_
