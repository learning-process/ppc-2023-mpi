// Copyright 2023 Zakharov Artem
#include "task_3/zakharov_a_multiple_integral_rec/multiple_integral.h"
#include <cmath>
#include <random>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/utility.hpp>

double calc_integ_par(std::size_t num_vars, func f,
                      const std::vector<std::size_t>& num_parts,
                      std::vector<std::pair<double, double>>* lim_integ) {
    boost::mpi::communicator comm;
    double global_res = 0;

    lim_integ->resize(num_vars);
    boost::mpi::broadcast(comm, lim_integ->data(),
                          static_cast<int>(num_vars), 0);

    std::size_t num_areas = get_num_areas(num_parts);
    std::size_t num_local_areas = num_areas / comm.size();

    std::vector<double> steps = get_steps(num_vars, *lim_integ, num_parts);

    std::vector<double> vars(num_vars);
    std::size_t first_area = num_local_areas * comm.rank();
    std::size_t last_area = first_area + num_local_areas;
    if (comm.rank() == comm.size() - 1) {
        last_area += num_areas % comm.size();
    }

    double local_res = calc_val_areas(first_area, last_area, num_vars, f,
                                      num_parts, *lim_integ, steps);

    boost::mpi::reduce(comm, local_res, global_res, std::plus(), 0);
    global_res = calc_volume(global_res, steps);
    return global_res;
}

double calc_integ_seq(std::size_t num_vars, func f,
                      const std::vector<std::size_t>& num_parts,
                      const std::vector<
                              std::pair<double, double>>& lim_integ) {
    std::size_t num_areas = get_num_areas(num_parts);

    std::vector<double> steps = get_steps(num_vars, lim_integ, num_parts);

    double res = calc_val_areas(0, num_areas, num_vars, f,
                                num_parts, lim_integ, steps);
    res = calc_volume(res, steps);
    return res;
}

double calc_val_areas(std::size_t first_area, std::size_t last_area,
                      std::size_t num_vars, func f,
                      const std::vector<std::size_t>& num_parts,
                      const std::vector<std::pair<double, double>>& lim_integ,
                      const std::vector<double>& steps) {
    double res = 0;
    double offset;
    std::vector<double> vars(num_vars);
    for (std::size_t cur_area = first_area; cur_area < last_area; cur_area++) {
        std::size_t ind_rest = cur_area;
        for (std::size_t var_ind = 0; var_ind < num_vars - 1; var_ind++) {
            std::size_t num_steps = ind_rest / num_parts[var_ind];
            offset = static_cast<double>(num_steps) * steps[var_ind] +
                    steps[var_ind] / 2;
            vars[var_ind] = lim_integ[var_ind].first + offset;
            ind_rest %= num_parts[var_ind];
        }
        offset = static_cast<double>(ind_rest) * steps[num_vars - 1] +
                steps[num_vars - 1] / 2;
        vars[num_vars - 1] = lim_integ[num_vars - 1].first + offset;
        res += f(vars);
    }
    return res;
}

std::size_t get_num_areas(const std::vector<std::size_t>& num_parts) {
    std::size_t number_areas = 1;
    for (std::size_t part : num_parts) {
        number_areas *= part;
    }
    return number_areas;
}

std::vector<double> get_steps(std::size_t num_vars, const std::vector<
        std::pair<double, double>>& lim_integ,
        const std::vector<std::size_t>& num_parts) {
    std::vector<double> steps(num_vars);
    for (int i = 0; i < num_vars; i++) {
        steps[i] = (lim_integ[i].second - lim_integ[i].first) /
                   static_cast<double>(num_parts[i]);
    }
    return steps;
}

double calc_volume(double sum_areas, const std::vector<double>& steps) {
    for (double step : steps) {
        sum_areas *= step;
    }
    return sum_areas;
}

std::pair<double, double> get_random_lim(int min_val, int max_val) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib1(min_val, max_val);
    int first = distrib1(gen);
    std::uniform_int_distribution<int>distrib2(first, max_val);
    return {first, distrib2(gen)};
}

double one_var_func(const std::vector<double>& vars) {
    return std::sin(vars[0]);
}

double two_var_func(const std::vector<double>& vars) {
    return std::pow(vars[0], 2) * std::pow(vars[1], 3);
}

double two_var_func_2(const std::vector<double>& vars) {
    return std::sin(vars[0]) + std::cos(vars[1]);
}

double three_var_func(const std::vector<double>& vars) {
    return std::pow(std::sin(vars[1]), 2) * std::cos(vars[0]) +
    std::sqrt(vars[2]);
}

double four_var_func(const std::vector<double>& vars) {
    return std::pow(vars[1] + vars[2], 3) + std::cos(vars[3]) * vars[0];
}
