// Copyright 2023 Sobol Lubov

#ifndef TASKS_TASK_1_SOBOL_L_MONTE_CARLO_METHOD_INTEGRATE_MONTE_CARLO_METHOD_INTEGRATE_H_
#define TASKS_TASK_1_SOBOL_L_MONTE_CARLO_METHOD_INTEGRATE_MONTE_CARLO_METHOD_INTEGRATE_H_

double calculateMonteCarloIntegral(double lowerBound, double upperBound, 
                                   double(*targetFunction)(double), int numberOfPoints);
#endif  // TASKS_TASK_1_SOBOL_L_MONTE_CARLO_METHOD_INTEGRATE_MONTE_CARLO_METHOD_INTEGRATE_H_
