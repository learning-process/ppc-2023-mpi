// Copyright 2023 kulagin_a
#ifndef TASKS_TASK_1_KULAGIN_A_FIND_MATRIX_COL_MIN_FIND_MATRIX_COL_MIN_H_
#define TASKS_TASK_1_KULAGIN_A_FIND_MATRIX_COL_MIN_FIND_MATRIX_COL_MIN_H_

#include <mpi.h>
#include <cstdlib>
#include <algorithm>

template<typename T>
inline T* generate_random_matrix(int rows, int cols, const T& min_val, const T& max_val) {
  int matrix_sz = cols * rows, i;
  T* matrix = new T[matrix_sz];
  for (i = 0; i < matrix_sz; i++) {
    matrix[i] = std::rand() % (max_val - min_val + 1) + min_val;
  }
  return matrix;
}

// copy matrix rows*cols from col_start to col_end
template<typename T>
inline void copy_matrix_cols(T* matrix, int rows, int cols, int col_start, int col_end, T* res) {
  int i, j, k = 0;
  for (j = 0; j < rows; j++) {
    for (i = col_start; i < col_end; i++) {
      res[k++] = matrix[i + j*cols];
    }
  }
}

template<typename T>
inline T* find_matrix_col_min_sequential(T* matrix, int rows, int cols, MPI_Datatype mpi_type = MPI_INT) {
  T* res = new T[cols];
  int matrix_sz = cols * rows;
  int i, j;
  T tmp;
  for (i = 0; i < cols; i++) {
    tmp = matrix[i];
    for (j = i; j < matrix_sz; j += cols) {
      if (tmp > matrix[j]) {
        tmp = matrix[j];
      }
    }
    res[i] = tmp;
  }
  return res;
}

template<typename T>
inline T* find_matrix_col_min_parallel(T* matrix, int rows, int cols, MPI_Datatype mpi_type = MPI_INT) {
  int i, j;
  int proc_rank, proc_num;
  T tmp;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  int delta = cols / proc_num;
  int delta_left = cols % proc_num;
  T* matrix_process = nullptr;
  T* res = nullptr;
  T* res_tmp = nullptr;
  int cols_now = (proc_rank != 0) ? delta : (delta + delta_left);
  int matrix_process_size = cols_now * rows;
  if (proc_rank == 0 || delta > 0) {
    matrix_process = new T[matrix_process_size];
  }
  if (proc_rank == 0) {
    res = new T[cols];
    if (delta > 0) {
      for (i = 1; i < proc_num; i++) {
        copy_matrix_cols<T>(matrix, rows, cols, delta_left + i * delta, delta_left + i * delta + delta, matrix_process);
        MPI_Send(matrix_process, delta * rows, mpi_type, i, 0, MPI_COMM_WORLD);
      }
    }
    copy_matrix_cols<T>(matrix, rows, cols, 0, cols_now, matrix_process);
  } else {
    if (delta > 0) {
      MPI_Recv(matrix_process, matrix_process_size, mpi_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  res_tmp = find_matrix_col_min_sequential<T>(matrix_process, rows, cols_now);
  if (proc_rank == 0) {
    std::copy(res_tmp, res_tmp + cols_now, res);
    delete[] res_tmp;
  } else {
    res = res_tmp;
  }
  if (matrix_process != nullptr) {
    delete[] matrix_process;
  }
  if (delta > 0) {
    if (proc_rank == 0) {
      for (i = 1; i < proc_num; i++) {
        MPI_Recv(res + delta * i + delta_left, delta, mpi_type, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    } else {
      MPI_Send(res, cols_now, mpi_type, 0, 0, MPI_COMM_WORLD);
      delete[] res;
      res = nullptr;
    }
  }
  return res;
}

#endif  // TASKS_TASK_1_KULAGIN_A_FIND_MATRIX_COL_MIN_FIND_MATRIX_COL_MIN_H_
