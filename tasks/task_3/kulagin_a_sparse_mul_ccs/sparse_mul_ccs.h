// Copyright 2023 kulagin_a
#ifndef TASKS_TASK_3_KULAGIN_A_SPARSE_MUL_CCS_SPARSE_MUL_CCS_H_
#define TASKS_TASK_3_KULAGIN_A_SPARSE_MUL_CCS_SPARSE_MUL_CCS_H_

#include <iostream>
#include <limits>
#include <vector>

// because floating point == is weird sometimes
template<typename T>
inline bool is_zero(const T& x) {
  return std::abs(x) <= std::numeric_limits<T>::epsilon();
}

// generate random value in range
template<typename T>
static inline T rand_range(const double& from, const double& to) {
  // because calculations with floaing points are weird sometimes
  double a = from + std::numeric_limits<double>::epsilon();
  double b = to - std::numeric_limits<double>::epsilon();
  // the formula
  return static_cast<T>(((static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * (b - a)) + a);
}

// struct to store sparse ccs mtrix
struct sparse_ccs_props {
  int rows = 0;
  int cols = 0;
  int vals = 0;
  double* vals_arr = nullptr;
  int* rows_ind_arr = nullptr;
  int* cols_count_arr = nullptr;
};

class sparse_ccs {
 protected:
  sparse_ccs_props props;
  bool do_del = false;
  friend sparse_ccs parallel_mul(sparse_ccs* m1, sparse_ccs* m2);

 public:
  sparse_ccs() {}
  sparse_ccs(const sparse_ccs& sm);
  sparse_ccs(sparse_ccs&& sm);
  sparse_ccs(const double* matrix, int rows, int cols);
  explicit sparse_ccs(sparse_ccs_props* props, bool do_del = true);
  sparse_ccs& operator=(const sparse_ccs& sm);
  sparse_ccs& operator=(sparse_ccs&& sm);
  ~sparse_ccs();
  void set_matrix(const double* matrix, int rows, int cols);
  void set_matrix(sparse_ccs_props* props, bool do_del = true);
  void set_do_del(bool do_del);
  void get_props(sparse_ccs_props* props);
  double* construct_matrix(void) const;
  int find_val_index(int i, int j) const;
  double get(int i, int j) const;
  int set_non_zero(int i, int j, const double& x);
  void clear(bool force = false);
  bool is_valid(void) const;
  bool operator==(const sparse_ccs& m) const;
  bool operator!=(const sparse_ccs& m) const;
  sparse_ccs operator*(const sparse_ccs& m) const;
  sparse_ccs transpose() const;
};

sparse_ccs parallel_mul(sparse_ccs* m1, sparse_ccs* m2);

sparse_ccs rand_sparse(int rows, int cols, double from = -100.0, double to = 100.0, double density = 0.1);

#endif  // TASKS_TASK_3_KULAGIN_A_SPARSE_MUL_CCS_SPARSE_MUL_CCS_H_
