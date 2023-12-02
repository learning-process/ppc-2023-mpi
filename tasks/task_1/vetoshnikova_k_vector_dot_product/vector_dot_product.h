#pragma once

#include <iostream> 
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

vector<int> vector_generation(int n);

int getParallelOperations(vector<int> global_vec_a, int count_size_vector_a,
	                      vector<int> global_vec_b, int count_size_vector_b);