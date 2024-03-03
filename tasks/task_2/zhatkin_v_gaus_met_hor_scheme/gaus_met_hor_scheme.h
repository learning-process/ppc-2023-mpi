// Copyright Zhatkin Vyacheslav

#ifndef TASKS_TASK_2_ZHATKIN_V_GAUS_MET_HOR_SCHEME_GAUS_MET_HOR_SCHEME_H_
#define TASKS_TASK_2_ZHATKIN_V_GAUS_MET_HOR_SCHEME_GAUS_MET_HOR_SCHEME_H_

#include <vector>

std::vector<double> newMatrix(int size);
std::vector<double> getGauss(const std::vector<double> &a, int size);
std::vector<double> getParGauss(const std::vector<double> &a, int size);
bool ChekTrueAnswer(const std::vector<double> &a, int size,
                    const std::vector<double> &x);

#endif  // TASKS_TASK_2_ZHATKIN_V_GAUS_MET_HOR_SCHEME_GAUS_MET_HOR_SCHEME_H_
