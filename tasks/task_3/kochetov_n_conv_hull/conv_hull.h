// Copyright 2023 Kochetov Nikolay
#ifndef TASKS_TASK_3_KOCHETOV_N_CONV_HULL_CONV_HULL_H_
#define TASKS_TASK_3_KOCHETOV_N_CONV_HULL_CONV_HULL_H_

#include <vector>

struct point {
  int x;
  int y;

  bool operator==(const point& other) {
    return (x == other.x && y == other.y);
  }

  bool operator!=(const point& other) {
    return !(*this == other);
  }
};

std::vector<point> find_hull(std::vector<std::vector<point>> cmpnts);
std::vector<std::vector<point>> marking(int* matrix, int size);

#endif  // TASKS_TASK_3_KOCHETOV_N_CONV_HULL_CONV_HULL_H_
