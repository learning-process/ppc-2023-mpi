// Copyright 2023 Kasimtcev Roman
#include "task_2/kasimtcev_reduce/reduce.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

static const char* incorrect_op_msg = "Incorrect MPI_Op op\n";
static const char* incorrect_type_msg = "Incorrect MPI_Datatype type\n";
static const int reduce_tag = 0;

typedef long double ldouble;

void Calculate(int Rank, int Num, proc_tree_props* props, int* path, int* path_size) {
  int x = 0, y = Num - 1, parent = -1;
  if (path_size) {
    *path_size = 0;
  }
  if (path) {
    std::fill(path, path + Num, -1);
  }
  while (x <= y) {
    int mid = (x + y) / 2;
    if (path && path_size) {
      path[(*path_size)++] = mid;
    }
    if (mid == Rank) {
      if (props) {
        props->parent = parent;
        int new_y = mid - 1, new_x = mid + 1;
        if (new_x <= y) {
          props->children[1] = (new_x + y) / 2;
        } else {
          props->children[1] = -1;
        }
        if (x <= new_y) {
          props->children[0] = (x + new_y) / 2;
        } else {
          props->children[0] = -1;
        }
      }
      return;
    } else if (mid > Rank) {
      y = mid - 1;
    } else {
      x = mid + 1;
    }
    parent = mid;
  }
}

static inline int cmp_child(const proc_tree_props& props, int child) {
  if (child == -1) {
    return -1;
  } else if (props.children[0] == child) {
    return 0;
  } else if (props.children[1] == child) {
    return 1;
  } else {
    return -1;
  }
}

template <typename T>
static inline int mpi_opp(const T *a, T *b, int count, MPI_Op op, bool print_err = true) {
#ifndef shortened_for_loop
#define shortened_for_loop(i, n) for ((i) = 0; (i) < (n); (i)++)
#endif
  int i, ret_status = MPI_SUCCESS;
  if (op == MPI_MAX) {
    shortened_for_loop(i, count) b[i] = std::max<T>(a[i], b[i]);
  } else if (op == MPI_MIN) {
    shortened_for_loop(i, count) b[i] = std::min<T>(a[i], b[i]);
  } else if (op == MPI_SUM) {
    shortened_for_loop(i, count) b[i] = a[i] + b[i];
  } else if (op == MPI_PROD) {
    shortened_for_loop(i, count) b[i] = a[i] * b[i];
  } else if (op == MPI_LAND) {
    shortened_for_loop(i, count) b[i] = a[i] && b[i];
  } else if (op == MPI_LOR) {
    shortened_for_loop(i, count) b[i] = a[i] || b[i];
  } else if (op == MPI_LXOR) {
    shortened_for_loop(i, count) b[i] = !a[i] != !b[i];
  } else {
    if (print_err) {
      std::cout << incorrect_op_msg;
    }
    ret_status = MPI_ERR_OP;
  }
#undef shortened_for_loop
  return ret_status;
}

template <typename T>
static inline int mpi_oppint(const T *a, T *b, int count, MPI_Op op) {
  int ret_status = mpi_opp<T>(a, b, count, op, false);
  if (ret_status != MPI_SUCCESS) {
    int i;
    ret_status = MPI_SUCCESS;
#ifndef shortened_for_loop
#define shortened_for_loop(i, n) for ((i) = 0; (i) < (n); (i)++)
#endif
    if (op == MPI_BAND) {
      shortened_for_loop(i, count) b[i] = a[i] & b[i];
    } else if (op == MPI_BOR) {
      shortened_for_loop(i, count) b[i] = a[i] | b[i];
    } else if (op == MPI_BXOR) {
      shortened_for_loop(i, count) b[i] = a[i] ^ b[i];
    } else {
      std::cout << incorrect_op_msg;
      ret_status = MPI_ERR_OP;
    }
#undef shortened_for_loop
  }
  return ret_status;
}

static inline int mpi_opper(const void *a, void *b, int count, MPI_Datatype type, MPI_Op op) {
  int ret;
  if (type == MPI_CHAR) {
    ret = mpi_oppint<char>(reinterpret_cast<const char *>(a), reinterpret_cast<char *>(b), count, op);
  } else if (type == MPI_SHORT) {
    ret = mpi_oppint<int16_t>(reinterpret_cast<const int16_t *>(a), reinterpret_cast<int16_t *>(b), count, op);
  } else if (type == MPI_LONG) {
    ret = mpi_oppint<int32_t>(reinterpret_cast<const int32_t *>(a), reinterpret_cast<int32_t *>(b), count, op);
  } else if (type == MPI_INT) {
    ret = mpi_oppint<int>(reinterpret_cast<const int *>(a), reinterpret_cast<int *>(b), count, op);
  } else if (type == MPI_UNSIGNED_CHAR) {
    ret = mpi_oppint<uint8_t>(reinterpret_cast<const uint8_t *>(a), reinterpret_cast<uint8_t *>(b), count, op);
  } else if (type == MPI_UNSIGNED_SHORT) {
    ret = mpi_oppint<uint16_t>(reinterpret_cast<const uint16_t *>(a), reinterpret_cast<uint16_t *>(b), count, op);
  } else if (type == MPI_UNSIGNED) {
    ret = mpi_oppint<unsigned>(reinterpret_cast<const unsigned *>(a), reinterpret_cast<unsigned *>(b), count, op);
  } else if (type == MPI_UNSIGNED_LONG) {
    ret = mpi_oppint<uint32_t>(reinterpret_cast<const uint32_t *>(a), reinterpret_cast<uint32_t *>(b), count, op);
  } else if (type == MPI_FLOAT) {
    ret = mpi_opp<float>(reinterpret_cast<const float *>(a), reinterpret_cast<float *>(b), count, op);
  } else if (type == MPI_DOUBLE) {
    ret = mpi_opp<double>(reinterpret_cast<const double *>(a), reinterpret_cast<double *>(b), count, op);
  } else if (type == MPI_LONG_DOUBLE) {
    ret = mpi_opp<ldouble>(reinterpret_cast<const ldouble *>(a), reinterpret_cast<ldouble *>(b), count, op);
  } else {
    std::cout << incorrect_type_msg;
    ret = MPI_ERR_TYPE;
  }
  return ret;
}

int mpi_reduce(const void *send, void *recv, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm) {
  int Rank, Num, type_sizeof, ret = MPI_SUCCESS, i;
  MPI_Comm_size(comm, &Num);
  MPI_Comm_rank(comm, &Rank);
  MPI_Type_size(type, &type_sizeof);
  if (root < 0 || root >= Num) {
    ret = MPI_ERR_ROOT;
    MPI_Abort(comm, ret);
    return ret;
  }
  if (count <= 0) {
    ret = MPI_ERR_COUNT;
    MPI_Abort(comm, ret);
    return ret;
  }
  if (send == nullptr || recv == nullptr) {
    ret = MPI_ERR_BUFFER;
    MPI_Abort(comm, ret);
    return ret;
  }

  proc_tree_props props;
  int* root_path = new int[Num];
  int root_path_sz;
  Calculate(root, Num, &props, root_path, &root_path_sz);
  if (Rank != root) {
    Calculate(Rank, Num, &props);
  }
  void* res;
  if (Rank != root) {
    res = std::malloc(count * type_sizeof);
  } else {
    res = recv;
  }
  memcpy(res, send, count * type_sizeof);
  int root_path_ind;
  if (Rank != root) {
    root_path_ind = std::find(root_path, root_path + root_path_sz, Rank) - root_path;
  } else {
    root_path_ind = root_path_sz - 1;
  }
  if (root_path_ind >= root_path_sz) {
    root_path_ind = -1;
  }
  int child_root_path = cmp_child(props, (root_path_ind == -1 || Rank == root) ? -1 : root_path[root_path_ind+1]);
  for (i = 0; i < 2; i++) {
    if (props.children[i] != -1 && i != child_root_path) {
      void* tmp = std::malloc(count * type_sizeof);
      MPI_Recv(tmp, count, type, props.children[i], 0, comm, MPI_STATUS_IGNORE);
      mpi_opper(tmp, res, count, type, op);
      std::free(tmp);
    }
  }
  if (root_path_ind == -1 && props.parent != -1) {
    MPI_Send(res, count, type, props.parent, 0, comm);
  }

  if (root_path_ind != -1 && props.parent != -1) {
    void* tmp = std::malloc(count * type_sizeof);
    MPI_Recv(tmp, count, type, props.parent, 1, comm, MPI_STATUS_IGNORE);
    mpi_opper(tmp, res, count, type, op);
    std::free(tmp);
  }

  if (child_root_path != -1) {
    MPI_Send(res, count, type, props.children[child_root_path], 1, comm);
  }

  delete[] root_path;
  if (Rank != root) {
    std::free(res);
  }
  MPI_Barrier(comm);
  return ret;
}
