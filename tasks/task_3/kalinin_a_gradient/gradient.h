// Copyright 2023 Kosarev Egor

#ifndef TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
#define TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_

#include <mpi.h>
#include <vector>
#include <random>

#define Vector std::vector < double >

Vector RandVec(int n);
Vector RandMat(int n);

double scalar_mult(const Vector& x,
	const Vector& y);

Vector mult_MxV(const Vector& M,
	const Vector& V);

Vector Serial_method_gradient(const Vector& M,
	const Vector& V, int n);

#endif  // TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
