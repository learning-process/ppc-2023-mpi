// Copyright 2023 Shubin Mikhail
#include "task_3/shubin_m_cannon_algorithm/cannon_algorithm.h"

double round_double(double val, int zeros) {
  return (static_cast<int>(val * pow(10, zeros)))/pow(10, zeros);
}

void mat_mult_cons(int n, const std::vector<double>& mat_in_1,
    const std::vector<double>& mat_in_2, std::vector<double>* mat_out) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        (*mat_out)[i*n+j] += mat_in_1[i*n+k] * mat_in_2[k*n+j];
      }
    }
  }
}

void mat_mult_cannon(int n, const std::vector<double>& mat_in_1,
    const std::vector<double>& mat_in_2, std::vector<double>* mat_out) {
  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int ProcNum_sqrt = static_cast<int>(sqrt(ProcNum));

  int n_ext = n % ProcNum_sqrt == 0 ? n : n + (ProcNum_sqrt - n % ProcNum_sqrt);

  std::vector<int> mat_size(ProcNum);
  std::vector<int> mat_displs(ProcNum);
  for (int i = 0; i < ProcNum; i++) {
    mat_size[i] = i < ProcNum_sqrt * ProcNum_sqrt ? (n_ext / ProcNum_sqrt) *
      (n_ext / ProcNum_sqrt) : 0;
  }
  mat_displs[0] = 0;
  for (int i = 1; i < ProcNum; i++) {
    mat_displs[i] = mat_size[i-1] + mat_displs[i-1];
  }
  int n_loc = static_cast<int>(sqrt(mat_size[ProcRank]));

  std::vector<double> loc_mat_1(mat_size[ProcRank]),
                      loc_mat_2(mat_size[ProcRank]),
                      t_mat_1(n_ext * n_ext),
                      t_mat_2(n_ext * n_ext),
                      res_loc(mat_size[ProcRank]),
                      res_glob(n_ext * n_ext);

  if (ProcRank == 0) {
    for (int row = 0; row < ProcNum_sqrt; row++) {
      for (int col = 0; col < ProcNum_sqrt; col++) {
        for (int i = 0; i < n_loc; i++) {
          for (int j = 0; j < n_loc; j++) {
            int k = (row + col) % ProcNum_sqrt,
                new_ind = (row * ProcNum_sqrt + col) * n_loc *
                  n_loc + (i * n_loc + j);

            if (row * n_loc + i < n && k * n_loc + j < n) {
              t_mat_1[new_ind] = mat_in_1[(row*n_loc+i)*n+(k*n_loc+j)];
            } else {
              t_mat_1[new_ind] = 0.0;
            }

            if (k * n_loc + i < n && col * n_loc + j < n) {
              t_mat_2[new_ind] = mat_in_2[(k*n_loc+i)*n+(col*n_loc+j)];
            } else {
              t_mat_2[new_ind] = 0.0;
            }
          }
        }
      }
    }
  }

  MPI_Scatterv(t_mat_1.data(), mat_size.data(), mat_displs.data(), MPI_DOUBLE,
               loc_mat_1.data(), mat_size[ProcRank], MPI_DOUBLE,
               0, MPI_COMM_WORLD);
  MPI_Scatterv(t_mat_2.data(), mat_size.data(), mat_displs.data(), MPI_DOUBLE,
               loc_mat_2.data(), mat_size[ProcRank], MPI_DOUBLE,
               0, MPI_COMM_WORLD);

  if (mat_size[ProcRank] != 0) {
    for (int q = 0; q < ProcNum_sqrt; q++) {
      mat_mult_cons(n_loc, loc_mat_1, loc_mat_2, &res_loc);
      if (q != ProcNum_sqrt - 1) {
        int ProcRow = ProcRank / ProcNum_sqrt,
            ProcCol = ProcRank % ProcNum_sqrt,
            send_trgt_1 = ProcRow * ProcNum_sqrt + (ProcCol - 1 +
                          ProcNum_sqrt) % ProcNum_sqrt,
            recv_src_1 = ProcRow * ProcNum_sqrt + (ProcCol + 1) % ProcNum_sqrt,
            send_trgt_2 = (ProcRow - 1 + ProcNum_sqrt) % ProcNum_sqrt *
                          ProcNum_sqrt + ProcCol,
            recv_src_2 = (ProcRow + 1) % ProcNum_sqrt * ProcNum_sqrt + ProcCol;

        MPI_Send(loc_mat_1.data(), mat_size[ProcRank], MPI_DOUBLE, send_trgt_1,
                 0, MPI_COMM_WORLD);
        MPI_Recv(loc_mat_1.data(), mat_size[ProcRank], MPI_DOUBLE, recv_src_1,
                 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Send(loc_mat_2.data(), mat_size[ProcRank], MPI_DOUBLE, send_trgt_2,
                 1, MPI_COMM_WORLD);
        MPI_Recv(loc_mat_2.data(), mat_size[ProcRank], MPI_DOUBLE, recv_src_2,
                 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }
  }

  MPI_Gatherv(res_loc.data(), mat_size[ProcRank], MPI_DOUBLE,
              res_glob.data(), mat_size.data(), mat_displs.data(), MPI_DOUBLE,
              0, MPI_COMM_WORLD);

  if (ProcRank == 0) {
    for (int row = 0; row < ProcNum_sqrt; row++) {
      for (int col = 0; col < ProcNum_sqrt; col++) {
        for (int i = 0; i < n_ext / ProcNum_sqrt; i++) {
          for (int j = 0; j < n_ext / ProcNum_sqrt; j++) {
            int new_col = col * n_loc + j,
                new_row = row * n_loc + i,
                old_ind = (row * ProcNum_sqrt + col) *
                           n_loc * n_loc + (i * n_loc + j);

            if (new_row < n && new_col < n) {
              (*mat_out)[new_row*n+new_col] = res_glob[old_ind];
            }
          }
        }
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
}
