#ifndef TASKS_TASK_3_LESNIKOV_GLOB_OPT_CHAR_PAR_H_
#define TASKS_TASK_3_LESNIKOV_GLOB_OPT_CHAR_PAR_H_

#include <iostream>
#include <functional>
#include <vector>


double findM(const std::vector<double>& X, const std::function<double(double)>& f);
double getm(double M, double r);
std::vector<double> getR(double m, const std::vector<double>& X, std::function<double(double)> f);
double getXk_1(double xt_1, double xt, double r, double m, std::function<double(double)> f);
int getMax(const std::vector<double>& v);
double getMinSequential(std::function<double(double)> f, double leftBound,
	double rightBound, double eps, int maxIterations, double r); 
double getMinParallel(std::function<double(double)> f, double leftBound,
	double rightBound, double eps, int maxIterations, double r); 



















std::vector<int> getRandomVector(int n);
std::vector<std::pair<size_t, size_t>> getSequentialMostDifferentElements(std::vector<int> v);
std::vector<std::pair<size_t, size_t>> getParallelMostDifferentElements(std::vector<int> v, int n);


#endif  // TASKS_TASK_3_LESNIKOV_GLOB_OPT_CHAR_PAR_H_