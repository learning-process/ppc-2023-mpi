// Copyright 2023 Ulyanov Daniil
#include "task_3/Ulyanov_d_find_min/find_min.h"

double getMParam(
  const std::vector<double>& x_cords,
  std::function<double(double)> func) {
  double param_M = 0;

  for (size_t i = 1; i < x_cords.size(); i++) {
    double f1 = func(x_cords[i]);
    double f0 = func(x_cords[i - 1]);
    double p = std::abs(x_cords[i] - x_cords[i - 1]);
    param_M = std::max(std::abs((f1 - f0) / p), param_M);
  }

  return param_M;
}
double getLipschitzParam(double param_M) {
  return (param_M > 0) ? 2.0 * param_M : 1;
}

std::vector<std::pair<int, double>> getIntervalCharactR(
  const std::vector<double>& x_cords,
  std::function<double(double)> func, double param_m) {
  std::vector<std::pair<int, double>> paramR;

  for (size_t i = 1; i < x_cords.size(); i++) {
    double z1 = func(x_cords[i]);
    double z0 = func(x_cords[i - 1]);
    double p = std::abs(x_cords[i] - x_cords[i - 1]);
    double curR = param_m * p + pow((z1 - z0), 2) / (param_m * p) - 2 * (z1 + z0);
    paramR.push_back(std::make_pair(i, curR));
  }

  return paramR;
}

double getNextPointX(double point_x0, double point_x1,
  std::function<double(double)> func, double param_m) {
  double z1 = func(point_x1);
  double z0 = func(point_x0);

  return point_x0 + (point_x1 - point_x0) / 2.0 + (z1 - z0) / (2.0 * param_m);
}

void sortVecR(std::vector<std::pair<int, double>>* chrct_R) {
  std::sort((*chrct_R).begin(), (*chrct_R).end(),
    [](const std::pair<int, double>& chR1,
      const std::pair<int, double>& chR2) {
        return chR1.second > chR2.second;
    });
}

double findMinSeq(double a, double b,
  std::function<double(double)> func, double E) {
  std::vector<double> sorted_x_cords = { a, b };

  while (sorted_x_cords.size() < 100000) {
    double M = getMParam(sorted_x_cords, func);
    double m = getLipschitzParam(M);
    auto chrct_R = getIntervalCharactR(sorted_x_cords, func, m);
    sortVecR(&chrct_R);

    int index_max_R = chrct_R[0].first;
    double next_point_x = getNextPointX(
      sorted_x_cords[index_max_R - 1],
      sorted_x_cords[index_max_R], func, m);

    if (std::abs(next_point_x - sorted_x_cords[index_max_R - 1]) < E)
      return func(next_point_x);

    sorted_x_cords.insert(std::upper_bound(
      sorted_x_cords.begin(),sorted_x_cords.end(),
      next_point_x), next_point_x);
  }
  return 9999999999999999999;
}

double findMinPar(double a, double b,
  std::function<double(double)> func, double E) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  std::vector<double> sorted_x_cords = { a, b };
  int iter_count = 0;

  while (iter_count++ < 100000) {
    std::vector<double> recv_buf(3);
    int count_work_proc = num;

    if (rank == 0) {
      double M = getMParam(sorted_x_cords, func);
      double m = getLipschitzParam(M);
      auto chrct_R = getIntervalCharactR(sorted_x_cords, func, m);

      sortVecR(&chrct_R);

      int indexMaxR = chrct_R[0].first;
      recv_buf = { sorted_x_cords[indexMaxR - 1], sorted_x_cords[indexMaxR], m };
      count_work_proc = std::min(static_cast<size_t>(num), chrct_R.size());

      for (int i = 1; i < count_work_proc; i++) {
        std::pair<int, double> part_R = chrct_R[i];
        std::vector<double> send_buf = {
            sorted_x_cords[part_R.first - 1],
            sorted_x_cords[part_R.first], m };
        MPI_Send(send_buf.data(), 3, MPI_DOUBLE, i,
          TAG_STATUS::SIMPLE_TAG, MPI_COMM_WORLD);
      }
    }

    MPI_Bcast(&count_work_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0 && rank < count_work_proc) {
      MPI_Status status;
      MPI_Recv(recv_buf.data(), 3, MPI_DOUBLE, 0,
        TAG_STATUS::SIMPLE_TAG, MPI_COMM_WORLD, &status);
    }

    bool res_find = false;
    double result = 0.0;
    if (rank < count_work_proc) {
      double next_point_x = getNextPointX(recv_buf[0], recv_buf[1], func, recv_buf[2]);
      int tag = TAG_STATUS::SEARCH_SOLUTION_TAG;

      if (std::abs(next_point_x - recv_buf[0]) < E) {
        tag = TAG_STATUS::FOUND_SOLUTION_TAG;
      }

      if (rank != 0) {
        MPI_Send(&next_point_x, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
      }
      else {
        for (int i = 1; i < count_work_proc; i++) {
          double recv_next_point_x;
          MPI_Status status;

          MPI_Recv(&recv_next_point_x, 1, MPI_DOUBLE, i,
            MPI_ANY_TAG, MPI_COMM_WORLD, &status);

          if (status.MPI_TAG == TAG_STATUS::FOUND_SOLUTION_TAG) {
            result = recv_next_point_x;
            res_find = true;
          }

          auto upBnd = std::upper_bound(
            sorted_x_cords.begin(), sorted_x_cords.end(), recv_next_point_x);
          sorted_x_cords.insert(upBnd, recv_next_point_x);
        }
        if (tag == TAG_STATUS::FOUND_SOLUTION_TAG) {
          result = next_point_x;
          res_find = true;
        }

        auto upBnd = std::upper_bound(
          sorted_x_cords.begin(), sorted_x_cords.end(), next_point_x);
        sorted_x_cords.insert(upBnd, next_point_x);
      }
    }

    MPI_Bcast(&res_find, 1, MPI_BYTE, 0, MPI_COMM_WORLD);

    if (res_find) {
      return func(result);
    }
  }

  return 9999999999999999999;
}