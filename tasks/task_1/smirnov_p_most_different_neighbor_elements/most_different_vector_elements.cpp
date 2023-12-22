// Copyright 2023 Smirnov Pavel
#include <mpi.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include "task_1/smirnov_p_most_different_neighbor_elements/most_different_vector_elements.h"


std::vector<int> getRandVect(int n) {
  std::random_device rd;
  std::mt19937 mersenne(rd());

  std::vector<int> vect;
  for (int i = 0; i < n; i++)
    vect.push_back(mersenne() % 10);
  return vect;
}

int single_find(std::vector<int> v) {
  if (v.size() - 1 < 2) {
    throw("small size of vector");
  }

  int max = 0;
  for (unsigned int i = 0; i < v.size() - 1; i++)
    if (abs(v[i] - v[i + 1]) > max) {
      max = abs(v[i] - v[i + 1]);
    }
  return max;
}

int mpi_find(std::vector<int> v) {
  if (v.size() - 1 < 2) {
    throw("small size of vector");
  }

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int step = 1, rest = 0;

  if ((unsigned int)size < v.size() - 1) {
    step = (v.size() - 1) / size;
    rest = (v.size() - 1) % size;
  }

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(&v[0] + proc * step + rest, step + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> localvect(step + 1);

  if (rank == 0) {
    localvect = std::vector<int>(v.begin(), v.begin() + step + rest + 1);
  } else {
    MPI_Status status;
    MPI_Recv(&localvect[0], step+1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

  int absmax = 0, localmax = 0;

  if ((unsigned int)rank < v.size() - 1) {
    for (size_t i = 0; i < localvect.size() - 1; i++) {
      if (abs(localvect[i] - localvect[i + 1]) > localmax) {
        localmax = abs(localvect[i] - localvect[i + 1]);
      }
    }
  }
  MPI_Reduce(&localmax, &absmax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  return absmax;
}


