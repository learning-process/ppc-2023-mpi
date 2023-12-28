// Copyright 2023 Trubin Artem
#ifndef TASKS_TASK_3_TRUBIN_A_BIN_IMG_BIN_IMG_H_
#define TASKS_TASK_3_TRUBIN_A_BIN_IMG_BIN_IMG_H_

#include <mpi.h>
#include <iostream>

struct image {
  int m, n, count;
  int** data;

  image(int _m, int _n) : m(_m), n(_n), count(1) {
    data = new int*[m];
    for (int i = 0; i < m; i++) {
      data[i] = new int[n];
      for (int j = 0; j < n; j++) {
        data[i][j] = 0;
      }
    }
  }

  friend std::ostream& operator<< (std::ostream& os, const image& img) {
    for (int i = 0; i < img.m; i++) {
      for (int j = 0; j < img.n; j++) {
        os << img.data[i][j] << "\t";
      }
      os << std::endl;
    }
    return os;
  }
};

void labeling(image* img);

#endif  // TASKS_TASK_3_TRUBIN_A_BIN_IMG_BIN_IMG_H_
