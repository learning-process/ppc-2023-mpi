// Copyright 2023 Smirnov Pavel
#ifndef TASKS_TASK_3_SMIRNOV_P_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
#define TASKS_TASK_3_SMIRNOV_P_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_

#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <cmath>
#include <iostream>

class Point2D {
 public:
    double x;
    double y;

    Point2D(double _x, double _y) {
        x = _x;
        y = _y;
    }

    friend bool operator<(const Point2D& firstPoint,
    const Point2D& secondPoint);
};

class Point3D {
 public:
    double x;
    double y;
    double z;

    Point3D(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    // when we fix oX in getGlobalMinimumOnPlane(),
    // we will need this constructor
    Point3D(double _x, Point2D point2D) {
        x = _x;
        y = point2D.x;
        z = point2D.y;
    }

    friend bool operator<(const Point3D& firstPoint,
    const Point3D& secondPoint);
};

class characteristicR {
 public:
    double R;
    double x;
    double z;
    double xPrevious;
    double zPrevious;

    characteristicR(double _R, double _x, double _z,
    double _xPrevious, double _zPrevious) {
        R = _R;
        x = _x;
        z = _z;
        xPrevious = _xPrevious;
        zPrevious = _zPrevious;
    }

    friend bool operator<(const characteristicR& r1,
    const characteristicR& r2) {
        return r1.R > r2.R;
    }
};

Point2D getGlobalMinimumOnSegment(double fixedVariable, double leftBorder,
    double rightBorder, double(*func)(double x, double y),
        int maxIterationsCount, double r, double accuracy);
Point3D getGlobalMinimumOnPlane(double xLeftBorder, double xRightBorder,
    double yBottomBorder, double yTopBorder, double(*func)(double x, double y),
        int maxIterationsCount, double r, double accuracy);
Point3D getGlobalMinimumOnPlaneParallelly(double xLeftBorder,
    double xRightBorder, double yBottomBorder, double yTopBorder,
        double(*func)(double x, double y), int maxIterationsCount, double r,
            double accuracy);

double function_1(double x, double y);

double function_2(double x, double y);

double function_3(double x, double y);

double function_4(double x, double y);

struct Point2DComparator {
    bool operator()(Point2D const& p1, Point2D const& p2)const {
        return p1.x < p2.x;
    }
};

struct Point3DComparator {
    bool operator()(Point3D const& p1, Point3D const& p2)const {
        return p1.x < p2.x;
    }
};


#endif  // TASKS_TASK_3_SMIRNOV_P_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
