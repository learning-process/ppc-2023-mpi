// Copyright 2023 Sredneva Anastasiya
#ifndef TASKS_TASK_2_SREDNEVA_A_HYPERCUBE_HYPERCUBE_H_
#define TASKS_TASK_2_SREDNEVA_A_HYPERCUBE_HYPERCUBE_H_

#include <mpi.h>
#include <vector>

int degree(int n);
std::vector <int> sm_matrix(int n);
std::vector<int> bfs(int n, int start, int end, const std::vector<int>& matr);
void send_data_hypercube(int data, int start, int end);
int next(int rank, const std::vector<int>& vec);
int prev(int rank, const std::vector<int>& vec);
bool inVec(int rank, const std::vector<int>& vec);

#endif  // TASKS_TASK_2_SREDNEVA_A_HYPERCUBE_HYPERCUBE_H_
