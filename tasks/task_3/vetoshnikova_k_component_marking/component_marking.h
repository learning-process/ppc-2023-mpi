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
};
void ParallelMarkingÑomponent(image* img);

#endif  // TASKS_TASK_3_VETOSHNIKOVA_K_COMPONENT_MARKING_COMPONENT_MARKING_H_
