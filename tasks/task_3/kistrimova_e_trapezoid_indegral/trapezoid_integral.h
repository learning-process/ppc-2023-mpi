// Copyright 2023 Kistrimova Ekaterina
#ifndef TASKS_TASK_3_KISTRIMOVA_E_TRAPEZOID_INDEGRAL_TRAPEZOID_INTEGRAL_H_
#define TASKS_TASK_3_KISTRIMOVA_E_TRAPEZOID_INDEGRAL_TRAPEZOID_INTEGRAL_H_

#include <vector>
#include <cmath>


class Func {
 protected:
  std::vector<double> l_bound, u_bound;

 public:
  virtual double CalculateFunc(std::vector<double> point) = 0;
  virtual std::vector<double> GetLowerBound();
  virtual std::vector<double> GetUpperBound();
};

class PiIntegral : public Func {
 public:
  PiIntegral();
  double CalculateFunc(std::vector<double> point);
};

class FirstIntegral : public Func {
 public:
  FirstIntegral();
  double CalculateFunc(std::vector<double> point);
};

class SecondIntegral : public Func {
 public:
  SecondIntegral();
  double CalculateFunc(std::vector<double> point);
};

class ThirdIntegral : public Func {
 public:
  ThirdIntegral();
  double CalculateFunc(std::vector<double> point);
};

double CalculateTrapezoidIntegral(Func* func,
                                  const std::vector<double>& l_bound,
                                  const std::vector<double>& u_bound,
                                  std::size_t segments_count,
                                  std::size_t f_point, std::size_t l_point);

double TrapezoidIntegralSequential(Func* func,
                                   const std::vector<double>& l_bound,
                                   const std::vector<double>& u_bound,
                                   std::size_t segments_count);

double TrapezoidIntegralParallel(Func* func,
                                 const std::vector<double>& l_bound,
                                 const std::vector<double>& u_bound,
                                 std::size_t segments_count);

#endif  // TASKS_TASK_3_KISTRIMOVA_E_TRAPEZOID_INDEGRAL_TRAPEZOID_INTEGRAL_H_
