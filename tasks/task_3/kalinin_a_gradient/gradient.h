// Copyright 2023 Kosarev Egor

#ifndef TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
#define TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_

#include <mpi.h>
#include <vector>

#define Vector std::vector < double >

Vector RandVec(int n);
Vector RandMat(int n);

double scalar_mult(const Vector& x,
	const Vector& y);
#endif  // TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
