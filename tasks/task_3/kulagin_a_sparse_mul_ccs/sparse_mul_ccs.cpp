// Copyright 2023 kulagin_a
#include "task_3/kulagin_a_sparse_mul_ccs/sparse_mul_ccs.h"
#include <mpi.h>
#include <limits>
#include <cmath>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <algorithm>

sparse_ccs::sparse_ccs(const sparse_ccs& sm) {
  operator=(sm);
}

sparse_ccs::sparse_ccs(sparse_ccs&& sm) {
  operator=(sm);
}

sparse_ccs::sparse_ccs(const double* matrix, int rows, int cols) {
  set_matrix(matrix, rows, cols);
}

sparse_ccs::sparse_ccs(sparse_ccs_props* props, bool do_del) {
  set_matrix(props, do_del);
}

sparse_ccs& sparse_ccs::operator=(const sparse_ccs& sm) {
  int i;
  clear();
  set_do_del(true);
  props.vals = sm.props.vals;
  props.rows = sm.props.rows;
  props.cols = sm.props.rows;
  props.vals_arr = new double[props.vals];
  props.rows_ind_arr = new int[props.vals];
  for (i = 0; i < props.vals; i++) {
    props.vals_arr[i] = sm.props.vals_arr[i];
    props.rows_ind_arr[i] = sm.props.rows_ind_arr[i];
  }
  props.cols_count_arr = new int[props.cols + 1];
  for (i = 0; i < props.cols + 1; i++) {
    props.cols_count_arr[i] = sm.props.cols_count_arr[i];
  }
  return *this;
}

sparse_ccs& sparse_ccs::operator=(sparse_ccs&& sm) {
  std::swap(props, sm.props);
  std::swap(do_del, sm.do_del);
  return *this;
}

sparse_ccs::~sparse_ccs() {
  clear();
}

void sparse_ccs::set_matrix(const double* matrix, int rows, int cols) {
  int i, j;
  clear();
  set_do_del(true);
  props.rows = rows;
  props.cols = cols;
  props.vals = 0;
  props.cols_count_arr = new int[cols + 1];
  props.cols_count_arr[0] = 0;
  for (j = 0; j < cols; j++) {
    props.cols_count_arr[j + 1] = props.cols_count_arr[j];
    for (i = 0; i < rows; i++) {
      if (!is_zero(matrix[j + i * cols])) {
        props.vals++;
        props.cols_count_arr[j + 1]++;
      }
    }
  }
  if (props.vals > 0) {
    int k = 0;
    props.rows_ind_arr = new int[props.vals];
    props.vals_arr = new double[props.vals];
    for (j = 0; j < cols; j++) {
      for (i = 0; i < rows; i++) {
        int ind = j + i * cols;
        if (!is_zero(matrix[ind])) {
          props.vals_arr[k] = matrix[ind];
          props.rows_ind_arr[k] = i;
          k++;
        }
      }
    }
  } else {
    clear(true);
    std::cout << "Cannot have 0 matrix\n";
  }
}

void sparse_ccs::set_matrix(sparse_ccs_props* props, bool do_del) {
  clear();
  this->props = *props;
  set_do_del(do_del);
}

void sparse_ccs::set_do_del(bool do_del) {
  // sometimes you don't actually need to free memory
  this->do_del = do_del;
}

void sparse_ccs::get_props(sparse_ccs_props* props) {
  *props = this->props;
}

double* sparse_ccs::construct_matrix(void) const {
  // construct dense matrix
  double* matrix = nullptr;
  if (is_valid()) {
    int i, matrix_sz = props.cols * props.rows;
    int col = 0, row, col_left = props.cols_count_arr[col + 1];
    matrix = new double[matrix_sz];
    std::memset(matrix, 0, matrix_sz * sizeof(double));
    for (i = 0; i < props.vals; i++) {
      while (col_left == 0) {
        col++;
        col_left = props.cols_count_arr[col + 1] - props.cols_count_arr[col];
      }
      row = props.rows_ind_arr[i];
      matrix[col + row * props.cols] = props.vals_arr[i];
      col_left--;
    }
  }
  return matrix;
}

int sparse_ccs::find_val_index(int i, int j) const {
  int ind = 0, col = 0, col_left = props.cols_count_arr[1];
  bool found = false;
  while (col <= j) {
    while (col_left == 0 && col <= j) {
      col++;
      col_left = props.cols_count_arr[col + 1] - props.cols_count_arr[col];
    }
    if (col == j && props.rows_ind_arr[ind] == i) {
      found = true;
      break;
    }
    ind++;
    col_left--;
  }
  if (!found) {
    ind = -1;
  }
  return ind;
}

double sparse_ccs::get(int i, int j) const {
  double val = 0.0;
  if (is_valid() && i >= 0 && j >= 0 && i < props.rows && j < props.cols) {
    int ind = find_val_index(i, j);
    if (ind != -1) {
      val = props.vals_arr[ind];
    }
  }
  return val;
}

int sparse_ccs::set_non_zero(int i, int j, const double& x) {
  int ret_status = 0;
  if (is_valid() && x != 0.0 && i >= 0 && j >= 0 && i < props.rows && j < props.cols) {
    int ind = find_val_index(i, j);
    if (ind != -1) {
        props.vals_arr[ind] = x;
    } else {
      ret_status = -1;
    }
  } else {
    ret_status = -2;
  }
  return ret_status;
}

void sparse_ccs::clear(bool force) {
  props.vals = 0;
  props.rows = 0;
  props.cols = 0;
  if (do_del || force) {
    if (props.vals_arr != nullptr) {
      delete[] props.vals_arr;
    }
    if (props.rows_ind_arr != nullptr) {
      delete[] props.rows_ind_arr;
    }
    if (props.cols_count_arr != nullptr) {
      delete[] props.cols_count_arr;
    }
  }
  props.vals_arr = nullptr;
  props.rows_ind_arr = nullptr;
  props.cols_count_arr = nullptr;
  do_del = false;
}

bool sparse_ccs::is_valid(void) const {
  // doesn't check props.cols_count_arr to be correct
  return
  (props.vals > 0 && props.rows > 0 && props.cols > 0 && props.vals_arr && props.rows_ind_arr && props.cols_count_arr);
}

bool sparse_ccs::operator==(const sparse_ccs& m) const {
  if (!is_valid() && !m.is_valid()) {
    return true;
  }
  if (!is_valid() || !m.is_valid()) {
    return false;
  }
  bool res = (props.vals == m.props.vals && props.cols == m.props.cols && props.rows == m.props.rows);
  if (res) {
    int i;
    for (i = 0; i < props.vals; i++) {
      res = (props.rows_ind_arr[i] == m.props.rows_ind_arr[i] && is_zero(props.vals_arr[i] - m.props.vals_arr[i]));
      if (!res) {
        return res;
      }
    }
    for (i = 0; i < props.cols + 1; i++) {
      res = (props.cols_count_arr[i] == m.props.cols_count_arr[i]);
      if (!res) {
        return res;
      }
    }
  }
  return res;
}

bool sparse_ccs::operator!=(const sparse_ccs& m) const {
  return !operator==(m);
}

sparse_ccs sparse_ccs::operator*(const sparse_ccs& m) const {
  if (props.cols != m.props.rows) {
    throw std::runtime_error("A.cols != B.rows");
  }
  if (!is_valid()) {
    throw std::runtime_error("A is invalid");
  }
  if (!m.is_valid()) {
    throw std::runtime_error("B is invalid");
  }
  int i, j;
  sparse_ccs_props res;
  res.rows = props.rows;
  res.cols = m.props.cols;
  res.cols_count_arr = new int[res.cols + 1];
  std::vector<int> temp_rows_ind_arr;
  temp_rows_ind_arr.reserve(std::max(props.vals, m.props.vals));
  std::vector<double> temp_vals_arr;
  temp_vals_arr.reserve(std::max(props.vals, m.props.vals));
  std::memset(res.cols_count_arr, 0, (res.cols + 1) * sizeof(int));
  sparse_ccs a = transpose();
  for (j = 0; j < res.cols; j++) {
    for (i = 0; i < res.rows; i++) {
      double sum = 0.0;
      int ks = a.props.cols_count_arr[i], kf = a.props.cols_count_arr[i+1];
      int ls = m.props.cols_count_arr[j], lf = m.props.cols_count_arr[j+1];
      while ((ks < kf) && (ls < lf)) {
        if (a.props.rows_ind_arr[ks] < m.props.rows_ind_arr[ls]) {
          ks++;
        } else if (a.props.rows_ind_arr[ks] > m.props.rows_ind_arr[ls]) {
          ls++;
        } else {
          sum += a.props.vals_arr[ks] * m.props.vals_arr[ls];
          ks++;
          ls++;
        }
      }
      if (!is_zero(sum)) {
        res.cols_count_arr[j + 1]++;
        temp_rows_ind_arr.push_back(i);
        temp_vals_arr.push_back(sum);
      }
    }
    res.cols_count_arr[j + 1] += res.cols_count_arr[j];
  }
  res.vals = temp_vals_arr.size();
  res.rows_ind_arr = new int[res.vals];
  res.vals_arr = new double[res.vals];
  std::copy(temp_rows_ind_arr.begin(), temp_rows_ind_arr.end(), res.rows_ind_arr);
  std::copy(temp_vals_arr.begin(), temp_vals_arr.end(), res.vals_arr);
  return sparse_ccs(&res, true);
}

sparse_ccs sparse_ccs::transpose() const {
  if (!is_valid()) {
    throw std::runtime_error("A is invalid");
  }
  int i, j;
  sparse_ccs_props res;
  res.rows = props.cols;
  res.cols = props.rows;
  res.vals = props.vals;
  // that's okay if we use a bit more memory (+1 element)
  res.cols_count_arr = new int[res.cols + 2];
  res.rows_ind_arr = new int[res.vals];
  res.vals_arr = new double[res.vals];
  std::memset(res.cols_count_arr, 0, (res.cols + 2) * sizeof(int));
  for (i = 0; i < props.vals; i++) {
    res.cols_count_arr[props.rows_ind_arr[i] + 2]++;
  }
  for (i = 2; i < res.cols + 2; i++) {
    res.cols_count_arr[i] += res.cols_count_arr[i-1];
  }
  for (i = 0; i < props.cols; i++) {
    for (j = props.cols_count_arr[i]; j < props.cols_count_arr[i+1]; j++) {
      const int ind = res.cols_count_arr[props.rows_ind_arr[j] + 1]++;
      res.vals_arr[ind] = props.vals_arr[j];
      res.rows_ind_arr[ind] = i;
    }
  }
  return sparse_ccs(&res, true);
}

sparse_ccs parallel_mul(sparse_ccs* m1, sparse_ccs* m2) {
  int proc_rank, proc_num;
  int i, j;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  if (proc_rank == 0 && m1->props.cols != m2->props.rows) {
    throw std::runtime_error("A.cols != B.rows");
  }
  if (proc_rank == 0) {
    if (!m1->is_valid()) {
      throw std::runtime_error("A is invalid");
    }
    if (!m2->is_valid()) {
      throw std::runtime_error("B is invalid");
    }
  }
  sparse_ccs a;
  sparse_ccs& m = *m2;
  sparse_ccs_props res;
  res.rows = m1->props.rows;
  res.cols = m2->props.cols;
  int rows = m1->props.rows;
  MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m, sizeof(m), MPI_BYTE, 0, MPI_COMM_WORLD);
  if (proc_rank == 0) {
    a = m1->transpose();
  }
  MPI_Bcast(&a, sizeof(a), MPI_BYTE, 0, MPI_COMM_WORLD);
  if (proc_rank != 0) {
    a.props.vals_arr = new double[a.props.vals];
    a.props.rows_ind_arr = new int[a.props.vals];
    a.props.cols_count_arr = new int[a.props.cols + 1];

    m.props.vals_arr = new double[m.props.vals];
    m.props.rows_ind_arr = new int[m.props.vals];
    m.props.cols_count_arr = nullptr;
  }
  MPI_Bcast(a.props.vals_arr, a.props.vals, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(a.props.rows_ind_arr, a.props.vals, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(m.props.vals_arr, m.props.vals, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(m.props.rows_ind_arr, m.props.vals, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(a.props.cols_count_arr, a.props.cols + 1, MPI_INT, 0, MPI_COMM_WORLD);
  int delta = (m.props.cols) / proc_num;
  int delta_left = (m.props.cols) % proc_num;
  int cols = (proc_rank == 0) ? delta + delta_left : delta;
  if (proc_rank == 0) {
    for (i = 1; i < proc_num; i++) {
      MPI_Send(m.props.cols_count_arr + delta_left + i * (delta), delta + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    m.props.cols_count_arr = new int[delta + 1];
    MPI_Recv(m.props.cols_count_arr, delta + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  std::vector<int> temp_rows_ind_arr;
  std::vector<double> temp_vals_arr;
  int* temp_cols_count_arr = new int[cols + 1];
  std::memset(temp_cols_count_arr, 0, (cols + 1) * sizeof(int));
  for (j = 0; j < cols; j++) {
    for (i = 0; i < rows; i++) {
      double sum = 0.0;
      int ks = a.props.cols_count_arr[i], kf = a.props.cols_count_arr[i+1];
      int ls = m.props.cols_count_arr[j], lf = m.props.cols_count_arr[j+1];
      while ((ks < kf) && (ls < lf)) {
        if (a.props.rows_ind_arr[ks] < m.props.rows_ind_arr[ls]) {
          ks++;
        } else if (a.props.rows_ind_arr[ks] > m.props.rows_ind_arr[ls]) {
          ls++;
        } else {
          sum += a.props.vals_arr[ks] * m.props.vals_arr[ls];
          ks++;
          ls++;
        }
      }
      if (!is_zero(sum)) {
        temp_cols_count_arr[j + 1]++;
        temp_rows_ind_arr.push_back(i);
        temp_vals_arr.push_back(sum);
      }
    }
    temp_cols_count_arr[j + 1] += temp_cols_count_arr[j];
  }
  int temp_vals = temp_vals_arr.size();
  a.clear();
  int* res_vals;
  if (proc_rank == 0) {
    res_vals = new int[proc_num];
  } else {
    m.clear(true);
  }
  MPI_Gather(&temp_vals, 1, MPI_INT, res_vals, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (proc_rank == 0) {
    for (i = 0; i < proc_num; i++) {
      res.vals += res_vals[i];
    }
    res.vals_arr = new double[res.vals];
    res.rows_ind_arr = new int[res.vals];
    res.cols_count_arr = new int[res.cols + 1];
    std::copy(temp_vals_arr.begin(), temp_vals_arr.end(), res.vals_arr);
    std::copy(temp_rows_ind_arr.begin(), temp_rows_ind_arr.end(), res.rows_ind_arr);
    std::copy(temp_cols_count_arr, temp_cols_count_arr + cols + 1, res.cols_count_arr);
    delete[] temp_cols_count_arr;
    int offset = res_vals[0];
    for (i = 1; i < proc_num; i++) {
      int ind = delta_left+(delta)*i+1;
      MPI_Recv(res.vals_arr + offset, res_vals[i], MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(res.rows_ind_arr + offset, res_vals[i], MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(res.cols_count_arr+ind, delta, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for (j = ind; j < ind + delta; j++) {
        res.cols_count_arr[j] += res.cols_count_arr[ind - 1];
      }
      offset += res_vals[i];
    }
    delete[] res_vals;
  } else {
    MPI_Send(temp_vals_arr.data(), temp_vals, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(temp_rows_ind_arr.data(), temp_vals, MPI_INT, 0, 1, MPI_COMM_WORLD);
    MPI_Send(temp_cols_count_arr+1 , delta, MPI_INT, 0, 2, MPI_COMM_WORLD);
    delete[] temp_cols_count_arr;
  }
  return sparse_ccs(&res, true);
}

sparse_ccs rand_sparse(int rows, int cols, double from, double to, double density) {
  int i;
  sparse_ccs_props res;
  if (is_zero(from) && is_zero(to)) {
    throw std::runtime_error("can't generate sparse matrix with values from 0 to 0");
  }
  if (rows <= 0 || cols <= 0) {
    throw std::runtime_error("invalid matrix size");
  }
  if (density <= 0.0 || density >= 1.0) {
    throw std::runtime_error("invalid density");
  }
  res.rows = rows;
  res.cols = cols;
  res.vals = static_cast<double>(rows * cols) * density;
  if (res.vals <= 0) {
    throw std::runtime_error("invalid density: too low");
  }
  res.vals_arr = new double[res.vals];
  res.rows_ind_arr = new int[res.vals];
  res.cols_count_arr = new int[res.cols + 1];
  for (i = 0; i < res.vals; i++) {
    double val;
    while (is_zero(val = rand_range<double>(from, to))) {}
    res.vals_arr[i] = val;
    res.rows_ind_arr[i] = rand_range<int>(0.0, static_cast<double>(rows - 1));
  }
  for (i = 1; i < res.cols; i++) {
    res.cols_count_arr[i] = rand_range<int>(0.0, static_cast<double>(res.vals));
  }
  res.cols_count_arr[0] = 0;
  res.cols_count_arr[res.cols] = res.vals;
  std::sort(res.cols_count_arr + 1, res.cols_count_arr + res.cols);
  return sparse_ccs(&res, true);
}
