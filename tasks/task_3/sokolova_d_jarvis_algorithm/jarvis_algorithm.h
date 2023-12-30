// Copyright 2023 Sokolova Daria
#ifndef TASKS_TASK_3_SOKOLOVA_D_JARVIS_ALGORITHM_JARVIS_ALGORITHM_H_
#define TASKS_TASK_3_SOKOLOVA_D_JARVIS_ALGORITHM_JARVIS_ALGORITHM_H_

#include <mpi.h>
#include <vector>

using std::vector;

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(const int _x, const int _y) : x(_x), y(_y) {}
};
vector<Point> randomVector(const vector<int>::size_type size);
vector<Point> consistentJarvisMarch(const vector<Point>& vectorOfVertex);
vector<Point> parallelJarvisMarch(const vector<Point>&
    vectorOfVertex, vector<int>::size_type vectorSize);

#endif  // TASKS_TASK_3_SOKOLOVA_D_JARVIS_ALGORITHM_JARVIS_ALGORITHM_H_
