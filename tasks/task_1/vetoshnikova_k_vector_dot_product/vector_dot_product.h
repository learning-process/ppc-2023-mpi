#pragma once
#include <iostream> 
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <mpi.h>

using namespace std;

vector<int> vector_generation(int mn, int mx, int sz);
void print_vector(vector<int> vec, int sz);

int getParallelOperations(vector<int> global_vec_a, vector<int> global_vec_b,
	                      int count_size_vector);

int getSequentialOperations(vector<int> a, vector<int> b,
	int count_size_vector);


