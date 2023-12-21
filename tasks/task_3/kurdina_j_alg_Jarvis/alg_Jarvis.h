// Copyright 2023 Kurdina Julia
#ifndef TASKS_TASK_3_KURDINA_J_ALG_JARVIS_ALG_JARVIS_H_
#define TASKS_TASK_3_KURDINA_J_ALG_JARVIS_ALG_JARVIS_H_

#include <mpi.h>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <utility>

struct point { int x, y;};
struct vec {
  int x, y;
  vec(point a, point b) {
    x = b.x - a.x;
    y = b.y - a.y;
  }
};
int cross(vec a, vec b);
std::vector<point> get_convex_hull(const std::vector<int>& image, int n, int m);
std::vector<point> get_convex_hull_parallel(const std::vector<int>& image, int n, int m);
bool check_answers(std::vector<point> not_par, std::vector<point> par);

#endif  // TASKS_TASK_3_KURDINA_J_ALG_JARVIS_ALG_JARVIS_H_
