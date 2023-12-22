// Copyright 2023 Lysanova Julia
#ifndef TASKS_TASK_3_LYSANOVA_J_GRAHAM_CONVEX_HULL_GRAHAM_CONVEX_HULL_H_
#define TASKS_TASK_3_LYSANOVA_J_GRAHAM_CONVEX_HULL_GRAHAM_CONVEX_HULL_H_

#include <vector>

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(const int _x, const int _y) : x(_x), y(_y) {}
};

std::vector<Point> GrahamMethod(std::vector<Point> vectorOfVertex);
std::vector<Point> parallelGrahamMethod(std::vector<Point> vectorOfVertex,
                                        std::vector<int>::size_type vectorSize);
std::vector<Point> random(const std::vector<int>::size_type Size);

#endif  // TASKS_TASK_3_LYSANOVA_J_GRAHAM_CONVEX_HULL_GRAHAM_CONVEX_HULL_H_
