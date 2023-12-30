// Copyright 2023 Kutarin Aleksandr
#include "task_1/kutarin_a_Integration_rectangle_method/integrating_rect.h"
#include <math.h>



double sin_func(double x) { return sin(x); }
double cos_func(double x) { return cos(x); }
double sin_cos(double x) { return sin(x) * cos(x); }
double hardfn_func(double x) { return 3 * x / std::sqrt(std::pow(x + 1, 3)); }
double hardfn2_func(double x) { return (std::pow(x, 3) - 2 * x +1)
    / std::pow(x+1, 2); }

double RectangleIntegrate(double left_border,
    double right_border, double step, func f) {
    if (left_border >= right_border) {
        return 0;
    }
    int num_procs, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double result = 0;
    int countintervals = static_cast<int>((right_border - left_border)
        / step);

    if (countintervals > 0) {
        double locsum = 0;
        double sum = 0;
        for (int i = my_rank; i < countintervals; i += num_procs) {
            sum += f(left_border + i * step + step / 2);
        }
        locsum = step * sum;
        MPI_Reduce(&locsum, &result, 1, MPI_DOUBLE,
             MPI_SUM, 0, MPI_COMM_WORLD);
    } else {
        return step * f((right_border + left_border) / 2);
    }
    return result;
}
