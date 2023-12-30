// Copyright 2023 Novostroev Ivan
#pragma once
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

#include <string>

std::vector<double> sequentialMul(std::vector<double> matrixa,
 std::vector<double> matrixb, int n);

void getRandMatrix(std::vector<double>* matrix, int N, int n);

bool isMatrEqual(std::vector<double> matrixa,
 std::vector<double> matrixb, int n);

void matrMalloc(double** matrix, int n);

void matrCalloc(double** matrix, int n);

void print_matr(std::vector<double> matrix, int Size);

std::vector<double> strassenAlgorithm(std::vector<double> matrixa,
 std::vector<double> matrixb, int n);
