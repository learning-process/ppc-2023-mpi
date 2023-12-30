// Copyright 2023 Tushentsova Karina
#ifndef TASKS_TASK_3_TUSHENTSOVA_K_CONVEX_HULL_CONVEX_HULL_H_
#define TASKS_TASK_3_TUSHENTSOVA_K_CONVEX_HULL_CONVEX_HULL_H_

#include <vector>

std::vector<int> getRandomVector();
std::vector<int> buildingConvexHull(
    const std::vector<int>& vecInput, int rows, int colls);
std::vector<int> parallBuildingConvexHull(
    std::vector<int> vecInput, int rows, int colls);
std::vector<int> building(const std::vector<int>& vecInput,
    int locrows, int rows, int colls, int rank);

#endif  // TASKS_TASK_3_TUSHENTSOVA_K_CONVEX_HULL_CONVEX_HULL_H_
