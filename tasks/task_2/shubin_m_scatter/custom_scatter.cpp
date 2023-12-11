// Copyright 2023 Shubin Mikhail
#include "task_2/shubin_m_scatter/custom_scatter.h"

int custom_scatter(const void *sendbuf, int sendcount,
                   MPI_Datatype sendtype,
                   void *recvbuf, int recvcount,
                   MPI_Datatype recvtype, int root,
                   MPI_Comm comm) {
  int err = MPI_SUCCESS;
  if (sendbuf == nullptr || recvbuf == nullptr) {
    err = MPI_ERR_BUFFER;
    MPI_Abort(comm, err);
    return err;
  }
  if (comm == MPI_COMM_NULL) {
    err = MPI_ERR_COMM;
    MPI_Abort(comm, err);
    return err;
  }
  if (sendcount <= 0 || recvcount <= 0) {
    err = MPI_ERR_COUNT;
    MPI_Abort(comm, err);
    return err;
  }
  if (recvtype != MPI_INT && recvtype != MPI_FLOAT && recvtype != MPI_DOUBLE) {
    err = MPI_ERR_TYPE;
    MPI_Abort(comm, err);
    return err;
  }

  int ProcRank, ProcNum;
  MPI_Comm_size(comm, &ProcNum);
  MPI_Comm_rank(comm, &ProcRank);

  if (ProcRank == root) {
    int send_size = 0;
    MPI_Type_size(sendtype, &send_size);
    int send_tot_size = sendcount * send_size;
    void* temp = const_cast<void*>(sendbuf);
    for (int i = 0; i < ProcNum; i++) {
      if (i != ProcRank) {
        err = MPI_Send(reinterpret_cast<char*>(temp) + (send_tot_size * i),
                       sendcount, sendtype, i, 0, comm);
        // Compiler needs size and const void* doesn't provide one
        // so we convert to char* to prevent error C2036
        if (err != MPI_SUCCESS) {
          MPI_Abort(comm, err);
          return err;
        }
      }
    }
    memcpy(recvbuf, sendbuf, send_tot_size);
  } else {
    err = MPI_Recv(recvbuf, recvcount, recvtype,
                   root, 0, comm, MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      MPI_Abort(comm, err);
      return err;
    }
  }

  return err;
}

int test_func(int val, bool custom_sc) {
  std::vector<int> val_vec, trsm_vec(1);
  int ProcRank, ProcNum;
  int sum = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < ProcNum; i++) {
    val_vec.push_back(val);
  }
  std::vector<int> res_vec(ProcNum);

  if (custom_sc) {
    custom_scatter(val_vec.data(), 1, MPI_INT,
                   trsm_vec.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(val_vec.data(), 1, MPI_INT, trsm_vec.data(),
                1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  std::vector<int> func_vec(1);
  func_vec[0] = trsm_vec[0];

  MPI_Gather(func_vec.data(), 1, MPI_INT, res_vec.data(),
             1, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ProcNum; i++) {
    sum += res_vec[i];
  }

  return sum;
}

float test_func(float val, bool custom_sc) {
  std::vector<float> val_vec, trsm_vec(1);
  int ProcRank, ProcNum;
  float sum = 0.0f;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < ProcNum; i++) {
    val_vec.push_back(val);
  }
  std::vector<float> res_vec(ProcNum);

  if (custom_sc) {
    custom_scatter(val_vec.data(), 1, MPI_FLOAT,
                   trsm_vec.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(val_vec.data(), 1, MPI_FLOAT, trsm_vec.data(),
                1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }

  std::vector<float> func_vec(1);
  func_vec[0] = trsm_vec[0];

  MPI_Gather(func_vec.data(), 1, MPI_FLOAT, res_vec.data(),
             1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ProcNum; i++) {
    sum += res_vec[i];
  }

  return sum;
}

double test_func(double val, bool custom_sc) {
  std::vector<double> val_vec, trsm_vec(1);
  int ProcRank, ProcNum;
  double sum = 0.0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < ProcNum; i++) {
    val_vec.push_back(val);
  }
  std::vector<double> res_vec(ProcNum);

  if (custom_sc) {
    custom_scatter(val_vec.data(), 1, MPI_DOUBLE,
                   trsm_vec.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(val_vec.data(), 1, MPI_DOUBLE, trsm_vec.data(),
                1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  std::vector<double> func_vec(1);
  func_vec[0] = trsm_vec[0];

  MPI_Gather(func_vec.data(), 1, MPI_DOUBLE, res_vec.data(),
             1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ProcNum; i++) {
    sum += res_vec[i];
  }

  return sum;
}

int test_func(const std::vector<int>& val_vec,
              int batch_size, bool custom_sc) {
  std::vector<int> trsm_vec(batch_size);
  int ProcRank, ProcNum;
  int sum = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<int> res_vec(ProcNum);
  double times[4];
  double time_custom = 0.0, time_default = 0.0;

  if (custom_sc) {
    if (ProcRank == 0) { times[0] = MPI_Wtime(); }
    custom_scatter(val_vec.data(), batch_size, MPI_INT,
                   trsm_vec.data(), batch_size, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) { times[1] = MPI_Wtime(); }
  } else {
    if (ProcRank == 0) { times[2] = MPI_Wtime(); }
    MPI_Scatter(val_vec.data(), batch_size, MPI_INT, trsm_vec.data(),
                batch_size, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) { times[3] = MPI_Wtime(); }
  }

  std::vector<int> func_vec(1);
  func_vec[0] = 0;
  for (int i = 0; i < batch_size; i++) {
    func_vec[0] += trsm_vec[i];
  }

  MPI_Gather(func_vec.data(), 1, MPI_INT, res_vec.data(),
             1, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ProcNum; i++) {
    sum += res_vec[i];
  }

  if (ProcRank == 0) {
    time_custom = abs(times[1] - times[0]);
    time_default = abs(times[3] - times[2]);
    std::cout << "Custom scatter time: " << time_custom << std::endl;
    std::cout << "Default scatter time: " << time_default << std::endl;
    std::cout << "Custom to default scatter time ratio: " <<
                 time_custom / time_default << std::endl;
  }

  return sum;
}

double test_func(const std::vector<double>& val_vec,
                 int batch_size, bool custom_sc) {
  std::vector<double> trsm_vec(batch_size);
  int ProcRank, ProcNum;
  double sum = 0.0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<double> res_vec(ProcNum);

  if (custom_sc) {
    custom_scatter(val_vec.data(), batch_size, MPI_DOUBLE,
                   trsm_vec.data(), batch_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(val_vec.data(), batch_size, MPI_DOUBLE, trsm_vec.data(),
                batch_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  std::vector<double> func_vec(1);
  func_vec[0] = 0.0;
  for (int i = 0; i < batch_size; i++) {
    func_vec[0] += trsm_vec[i];
  }

  MPI_Gather(func_vec.data(), 1, MPI_DOUBLE, res_vec.data(),
             1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ProcNum; i++) {
    sum += res_vec[i];
  }

  return sum;
}
