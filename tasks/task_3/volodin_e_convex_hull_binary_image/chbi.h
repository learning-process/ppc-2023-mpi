// Copyright 2023 Volodin Evgeniy
#ifndef TASKS_TASK_3_VOLODIN_E_CONVEX_HULL_BINARY_IMAGE_CHBI_H_
#define TASKS_TASK_3_VOLODIN_E_CONVEX_HULL_BINARY_IMAGE_CHBI_H_
#include <mpi.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <cmath>
#include <string>
#include <map>
#include <functional>
#include <random>

struct point2d {
    int x, y;
    point2d(int x, int y) : x(x), y(y) {}
};

std::vector<int> findComponents(const std::vector<std::vector<int>>& image, int width, int height);
void floodFill(std::vector<int>* image, int height, int width, int yStart, int xStart, int label);
int findCountComponents(const std::vector<int> &image);
int findCountPointsInComponent(const std::vector<int> &image);
std::vector<int> removeExtraPoints(const std::vector<int> &image, int width, int height, int component);
void sort(std::vector<int>* points, int xMin, int yMin);
std::vector<int> graham(std::vector<int> points);
std::vector<int> getConvexHullSeq(const std::vector<std::vector<int>> &image, int width, int height);
std::vector<int> getConvexHullPar(const std::vector<std::vector<int>> &image, int width, int height);
void fillImageRandom(std::vector<std::vector<int>>* image, int width, int height);

#endif  // TASKS_TASK_3_VOLODIN_E_CONVEX_HULL_BINARY_IMAGE_CHBI_H_
