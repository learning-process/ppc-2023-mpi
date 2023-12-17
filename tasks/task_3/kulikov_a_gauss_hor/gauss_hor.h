// Copyright 2023 Kulikov Artem
#ifndef TASKS_TASK_3_KULIKOV_A_GAUSS_HOR_GAUSS_HOR_H_
#define TASKS_TASK_3_KULIKOV_A_GAUSS_HOR_GAUSS_HOR_H_

#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<double> get3x3GaussKernel(double sd);
uint32_t getPxSum(const std::vector<uint8_t>& pic, int n);
std::vector<uint8_t> getExtPicture(int n, int m);
std::vector<uint8_t> getParallelGauss(const std::vector<uint8_t>& orig,
                                        const std::vector<double>& kernel, int n, int m);
std::vector<uint8_t> getSequentialGauss(const std::vector<uint8_t>& orig,
                                        const std::vector<double>& kernel, int n);

#endif  // TASKS_TASK_3_KULIKOV_A_GAUSS_HOR_GAUSS_HOR_H_
