// Copyright 2023 Kazantsev Evgeny
#ifndef TASKS_TASK_2_KAZANTSEV_E_MUL_TAPE_MATRIX_MUL_TAPE_MATRIX_H_
#define TASKS_TASK_2_KAZANTSEV_E_MUL_TAPE_MATRIX_MUL_TAPE_MATRIX_H_
#include <vector>
#include <string>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::vector<int> GetMatrixRand(int n);
std::vector<int > SequentialMulMatrix(const std::vector<int>& a,
const std::vector<int>& b, int n, int m, int m1);
std::vector<int> parallelMatrixMul(std::vector<int>* c,
std::vector<int>* d, int an, int am);
#endif  // TASKS_TASK_2_KAZANTSEV_E_MUL_TAPE_MATRIX_MUL_TAPE_MATRIX_H_
