// Copyright 2023 Nikitin Kirill
#include "task_2/nikitin_k_simple_iterate/simple_iterate.h"

std::vector<double> SimpleIterSeq(std::vector<double> a, std::vector<double> b,
                                  double epsilon) {
  const size_t size_a = a.size();
  const size_t size_b = b.size();

  if (size_a != size_b * size_b || size_b == 0) return std::vector<double>();

  size_t n = size_b;
  double current = 0.0;
  double norm = 0.0;

  for (int i = 0; i < n; ++i) {
    current = a[n * i + i];
    if (current == 0) return std::vector<double>();

    b[i] /= current;

    for (int j = 0; j < n; ++j) {
      if (i == j)
        a[n * i + j] = 0.0;
      else
        a[n * i + j] /= current;

      if (std::abs(a[n * i + j]) > norm) norm = std::abs(a[n * i + j]);
    }
  }

  if (norm++ >= 1) return std::vector<double>();

  std::vector x_prev(b);
  std::vector x(b);

  while (norm > epsilon) {
    x_prev = x;
    x = b;
    norm = 0.0;

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) x[i] -= a[n * i + j] * x_prev[j];

      if (std::abs(x[i] - x_prev[i]) > norm) norm = std::abs(x[i] - x_prev[i]);
    }
  }
  return x;
}

std::vector<double> SimpleIterPar(std::vector<double> a, std::vector<double> b,
                                  double epsilon, MPI_Comm comm) {
  const size_t size_b = b.size();
  const size_t size_a = a.size();

  if (size_a != size_b * size_b || size_b == 0) return std::vector<double>();

  int rank = 0;
  int size = 0;
  size_t n = size_b;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  if (n < size) return SimpleIterSeq(a, b, epsilon);

  int delta = n / size;
  int remain = n % size;

  std::vector<int> count_send_a(size, delta * n);
  std::vector<int> count_send_b(size, delta);

  std::vector<int> displs_a(size, 0);
  std::vector<int> displs_b(size, 0);

  for (int i = 1; i < size; i++) {
    displs_a[i] = static_cast<int>(n) * (remain + delta * i);
    displs_b[i] = remain + delta * i;
  }

  count_send_a[0] = static_cast<int>(n) * (remain + delta);
  count_send_b[0] = remain + delta;

  std::vector<double> local_a(count_send_a[rank]);
  std::vector<double> local_b(count_send_b[rank]);

  std::vector<double> local_x(count_send_a[rank], 0);
  std::vector<double> x(n);

  MPI_Scatterv(a.data(), count_send_a.data(), displs_a.data(), MPI_DOUBLE,
               local_a.data(), count_send_a[rank], MPI_DOUBLE, 0, comm);
  MPI_Scatterv(b.data(), count_send_b.data(), displs_b.data(), MPI_DOUBLE,
               local_b.data(), count_send_b[rank], MPI_DOUBLE, 0, comm);

  double diag_val = 0.0;
  double glob_norm = 1.0;
  double loc_norm = 0.0;

  for (int i = 0; i < count_send_b[rank]; ++i) {
    diag_val = local_a[displs_b[rank] + n * i + i];
    if (diag_val == 0) return std::vector<double>();

    local_b[i] /= diag_val;

    for (int j = 0; j < n; j++) {
      if (displs_b[rank] + i == j)
        local_a[n * i + j] = 0;
      else
        local_a[n * i + j] /= diag_val;

      if (std::abs(local_a[n * i + j]) > loc_norm) {
        loc_norm = std::abs(local_a[n * i + j]);
        if (loc_norm >= 1) return std::vector<double>();
      }
    }
  }

  MPI_Allgatherv(local_b.data(), count_send_b[rank], MPI_DOUBLE, x.data(),
                 count_send_b.data(), displs_b.data(), MPI_DOUBLE, comm);

  while (glob_norm > epsilon) {
    loc_norm = 0;

    for (int i = 0; i < count_send_b[rank]; i++) {
      local_x[i] = local_b[i];

      for (int j = 0; j < n; j++) local_x[i] -= local_a[n * i + j] * x[j];

      if (std::abs(local_x[i] - x[displs_b[rank] + i]) > loc_norm)
        loc_norm = std::abs(local_x[i] - x[displs_b[rank] + i]);
    }
    MPI_Allreduce(&loc_norm, &glob_norm, 1, MPI_DOUBLE, MPI_MAX, comm);
    MPI_Allgatherv(local_x.data(), count_send_b[rank], MPI_DOUBLE, x.data(),
                   count_send_b.data(), displs_b.data(), MPI_DOUBLE, comm);
  }
  return x;
}
