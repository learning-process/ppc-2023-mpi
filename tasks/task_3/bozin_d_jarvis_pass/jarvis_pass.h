// Copyright 2023 Bozin Dmitry
#ifndef TASKS_TASK_3_BOZIN_D_JARVIS_PASS_JARVIS_PASS_H_
#define TASKS_TASK_3_BOZIN_D_JARVIS_PASS_JARVIS_PASS_H_

#include <mpi.h>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <utility>

struct point { int x, y; };
struct vec {
  int x, y;
  vec(point a, point b) {
    x = b.x - a.x;
    y = b.y - a.y;
  }
};
int cross(vec a, vec b);
std::vector<point> getConvexHull(const std::vector<int>& image, int n, int m);
std::vector<point> getConvexHullParallel(const std::vector<int>& image, int n, int m);
bool checkAnswers(std::vector<point> not_par, std::vector<point> par);

#endif  // TASKS_TASK_3_BOZIN_D_JARVIS_PASS_JARVIS_PASS_H_
