// Copyright 2023 Mirzakhmedov Alexander

#pragma once
#include <vector>
#include <random>

using std::vector;
using std::mt19937;

vector<double> GaussConsequent(int matrix_size);
void UpdateRandNumbers(mt19937 *gen);
vector<double> CreateMatrix(int size);
vector<double> CreateVector(int size);
vector<double> CreateMatrixRandom(int size, mt19937 *gen);
vector<double> CreateVectorRandom(int size, mt19937 *gen);
vector<double> GaussParallel(const vector<double> &matr, const vector<double> &right_part, int size_matr);
vector<double> GaussParallels(const vector<double> &matr, int size_matr);
vector<double> UnionMatrVect(const vector<double> &matr, const vector<double> &right_vec, int size_matr);
void PrintMatrixVector(const vector<double> &matr, int size_matr);
void PrintVector(const vector<double> &vec, int vec_size);
