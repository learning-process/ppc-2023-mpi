// Copyright 2023 Kistrimova Ekaterina
#include <mpi.h>
#include <vector>
#include "task_3/kistrimova_e_trapezoid_indegral/trapezoid_integral.h"

std::vector<double> Func::GetLowerBound() { return l_bound; }
std::vector<double> Func::GetUpperBound() { return u_bound; }

PiIntegral::PiIntegral() {
  l_bound = {0.0};
  u_bound = {1.0};
}

double PiIntegral::CalculateFunc(std::vector<double> point) {
  return 4.0 / (1.0 + point[0] * point[0]);
}

FirstIntegral::FirstIntegral() {
  l_bound = {0.0, 0.0};
  u_bound = {1.0, 1.0};
}

double FirstIntegral::CalculateFunc(std::vector<double> point) {
  return point[0] * point[0] + point[1] * point[1];
}

SecondIntegral::SecondIntegral() {
  l_bound = {0.0, 0.0};
  u_bound = {2.0, 2.0};
}

double SecondIntegral::CalculateFunc(std::vector<double> point) {
  return point[0] * point[1];
}

ThirdIntegral::ThirdIntegral() {
  l_bound = {-1.0, -1.0, -1.0};
  u_bound = {1.0, 1.0, 1.0};
}

double ThirdIntegral::CalculateFunc(std::vector<double> point) {
  return sin(point[0]) + cos(point[1]) + sin(point[2]);
}

double CalculateTrapezoidIntegral(Func* func,
                                  const std::vector<double>& l_bound,
                                  const std::vector<double>& u_bound,
                                  std::size_t segments_count,
                                  std::size_t f_point, std::size_t l_point) {
  std::size_t variables_count = l_bound.size();
  std::vector<double> height(variables_count);
  double res = 0.0;

  std::vector<double> curr_seg(variables_count);
  std::size_t tmp_p = f_point;
  for (std::size_t i = 0; i < variables_count; i++) {
    curr_seg[i] = tmp_p % segments_count;
    tmp_p /= segments_count;
  }

  for (std::size_t i = 0; i < variables_count; i++)
    height[i] = (u_bound[i] - l_bound[i]) / segments_count;

  std::vector<double> prev_point(variables_count),
    curr_point(variables_count);
  for (std::size_t i = f_point; i < l_point; i++) {
    for (std::size_t j = 0; j < variables_count; j++) {
      prev_point[j] = l_bound[j] + curr_seg[j] * height[j];
      curr_point[j] = prev_point[j] + height[j];
    }
    res +=
        (func->CalculateFunc(prev_point) +
          func->CalculateFunc(curr_point)) / 2;
    curr_seg[0] += 1;
    for (std::size_t j = 0; j < variables_count - 1; j++) {
      if (curr_seg[j] == segments_count) {
        curr_seg[j] = 0;
        ++curr_seg[j + 1];
      } else {
        break;
      }
    }
  }
  for (std::size_t i = 0; i < variables_count; i++) res *= height[i];

  return res;
}

double TrapezoidIntegralSequential(Func* func,
                                   const std::vector<double>& l_bound,
                                   const std::vector<double>& u_bound,
                                   std::size_t segments_count) {
  std::size_t points_count = 1, variables_count = l_bound.size();
  for (std::size_t i = 0; i < variables_count; i++)
    points_count *= segments_count;
  return CalculateTrapezoidIntegral(func, l_bound, u_bound,
                                    segments_count, 0, points_count);
}

double TrapezoidIntegralParallel(Func* func,
                                 const std::vector<double>& l_bound,
                                 const std::vector<double>& u_bound,
                                 std::size_t segments_count) {
  int procSize, procrank;
  MPI_Comm_size(MPI_COMM_WORLD, &procSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &procrank);

  std::size_t variables_count = l_bound.size(), points_count = 1;

  for (std::size_t i = 0; i < variables_count; i++)
    points_count *= segments_count;

  std::size_t local_segment_size = points_count / procSize;
  std::size_t remenian_size = points_count % procSize;
  std::size_t f_point = procrank * local_segment_size + remenian_size;
  std::size_t l_point = (procrank + 1) * local_segment_size + remenian_size;

  if (procrank == 0) f_point = 0;

  double local_integral = CalculateTrapezoidIntegral(
      func, l_bound, u_bound, segments_count, f_point, l_point);

  double res = 0.0;

  MPI_Reduce(&local_integral, &res, 1, MPI_DOUBLE,
    MPI_SUM, 0, MPI_COMM_WORLD);

  return res;
}
