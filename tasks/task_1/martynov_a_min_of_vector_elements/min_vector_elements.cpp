// Copyright 2023 Martynov Aleksandr
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "tasks/task_1/martynov_a_min_of_vector_elements/min_vector_elements.h"

int get_minimal_elem(const std::vector<int>& init_vec, size_t vec_size) {
  if (vec_size == 0)
    return 0;
  int min_element_in_thread = 1e+10;
  int global_min_elem;
  int myrank;
  int count_of_proc;
  int elements_per_proc = 0;
  int count_of_elements_on_current_proc = 0;
  int remainder = 0;
  double* global_array = nullptr;
  double* get_array = nullptr;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  std::vector<int> A1(count_of_proc);
  std::vector<int> A2(count_of_proc);
  std::vector<int> B1(count_of_proc);
  std::vector<int> B2(count_of_proc);

  elements_per_proc = vec_size / count_of_proc;
  remainder = vec_size % count_of_proc;

  if (myrank == 0) {
    global_array = new double[vec_size];
    for (int i = 0; i < vec_size; i++)
      global_array[i] = init_vec[i];
    for (int q = 0; q < count_of_proc; q++) {
      A1[q] = elements_per_proc;
      A2[q] = elements_per_proc;
      B1[q] = 0;
      B2[q] = 0;
    }
    A1[count_of_proc - 1] += remainder;
    A2[count_of_proc - 1] += remainder;
    for (int q = 1; q < count_of_proc; q++) {
      B1[q] = B1[q - 1] + A1[q - 1];
      B2[q] = B2[q - 1] + A2[q - 1];
    }
    get_array = new double[A2[0]];
  } else {
    if (myrank == (count_of_proc - 1)) {
      elements_per_proc = elements_per_proc + remainder;
      get_array = new double[elements_per_proc];
    } else {
      get_array = new double[elements_per_proc];
    }
  }
  MPI_Scatterv(global_array, A1.data(), B1.data(),
    MPI_DOUBLE, get_array, elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int q = 0; q < elements_per_proc; q++) {
    if (min_element_in_thread > get_array[q])
      min_element_in_thread = get_array[q];
  }
  MPI_Reduce(&min_element_in_thread, &global_min_elem, 1,
    MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  return global_min_elem;
}
void get_random_vector(std::vector<int>* vector, size_t size,
  int bot_elem, int top_elem) {
  std::mt19937 gen(1233);
  std::uniform_int_distribution<> distr(bot_elem, top_elem);
  for (int q = 0; q < size; q++)
    vector->push_back(distr(gen));
}
