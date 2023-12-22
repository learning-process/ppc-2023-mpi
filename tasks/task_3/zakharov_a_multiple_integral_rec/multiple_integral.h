// Copyright 2023 Zakharov Artem
#ifndef TASKS_TASK_3_ZAKHAROV_A_MULTIPLE_INTEGRAL_REC_MULTIPLE_INTEGRAL_H_
#define TASKS_TASK_3_ZAKHAROV_A_MULTIPLE_INTEGRAL_REC_MULTIPLE_INTEGRAL_H_
#include <vector>
#include <utility>
#include <cstddef>

typedef double (*func)(const std::vector<double>&);

double calc_integ_par(std::size_t num_vars, func f,
                      const std::vector<std::size_t>& num_parts,
                      std::vector<std::pair<double, double>>* lim_integ);

double calc_integ_seq(std::size_t num_vars, func f,
                      const std::vector<std::size_t>& num_parts,
                      const std::vector<std::pair<double, double>>& lim_integ);
double calc_val_areas(std::size_t first_area, std::size_t last_area,
                      std::size_t num_vars, func f,
                      const std::vector<std::size_t>& num_parts,
                      const std::vector<std::pair<double, double>>& lim_integ,
                      const std::vector<double>& steps);

double calc_volume(double sum_areas, const std::vector<double>& steps);

std::size_t get_num_areas(const std::vector<std::size_t>& num_parts);

std::vector<double> get_steps(std::size_t num_vars, const std::vector<
        std::pair<double, double>>& lim_integ,
        const std::vector<std::size_t>& num_parts);

std::pair<double, double> get_random_lim(int min_val, int max_val);

double one_var_func(const std::vector<double>& vars);
double two_var_func(const std::vector<double>& vars);
double two_var_func_2(const std::vector<double>& vars);
double three_var_func(const std::vector<double>& vars);
double four_var_func(const std::vector<double>& vars);

#endif  // TASKS_TASK_3_ZAKHAROV_A_MULTIPLE_INTEGRAL_REC_MULTIPLE_INTEGRAL_H_
