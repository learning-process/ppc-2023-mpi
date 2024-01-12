// Copyright 2023 Vetoshnikova Ekaterina


#ifndef TASKS_TASK_3_VETOSHNIKOVA_K_COMPONENT_MARKING_COMPONENT_MARKING_H_
#define TASKS_TASK_3_VETOSHNIKOVA_K_COMPONENT_MARKING_COMPONENT_MARKING_H_

#include <mpi.h>
#include <iostream>

struct image {
    int m, n, count;
    int** data;

    image(int _m, int _n) {
        m = _m;
        n = _n;
        count = 0;
        data = new int* [m];
        for (int i = 0; i < m; i++) {
            data[i] = new int[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = 0;
            }
        }
    }
    image(const image& img) {
        m = img.m;
        n = img.n;
        count = img.count;
        data = new int* [m];
        for (int i = 0; i < m; i++) {
            data[i] = new int[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = img.data[i][j];
            }
        }
    }
    image& operator=(image img) {
        if (this == &img) return img;
        for (int v = 0; v < m; v++) {
            delete data[v];
        }
        delete[]data;
        m = img.m;
        n = img.n;
        count = img.count;
        data = new int* [m];
        for (int i = 0; i < m; i++) {
            data[i] = new int[n];
            for (int k = 0; k < n; k++) {
                data[i][k] = img.data[i][k];
            }
            return *this;
        }
    }
    ~image() {
        delete[]data[0];
        delete[]data;
    }
};
void ParallelMarkingComponent(image* img);

#endif  // TASKS_TASK_3_VETOSHNIKOVA_K_COMPONENT_MARKING_COMPONENT_MARKING_H_
