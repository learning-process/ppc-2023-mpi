// Copyright 2023 Suren Simonyan
#include "task_1/simonyan_a_integrating_rectangle_method/integrating_rect.h"
#include <math.h>
double sin_f(double x) { return sin(x); }
double cos_f(double x) { return cos(x); }
double hardfn_f(double x) { return 3 * x / std::sqrt(std::pow(x + 1, 3)); }
double hardfn2_f(double x) { return (std::pow(x, 3) - 2 * x +1)
    / std::pow(x+1, 2); }
double sin_cos(double x) { return sin(x) * cos(x); }

double RectangleIntegrate(double leftbound,
    double rightbound, double step, func f) {
    if (leftbound >= rightbound) {
        return 0;
    }
    int num_procs, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double result = 0;
    int countintervals = static_cast<int>((rightbound - leftbound) / step);

    if (countintervals > 0) {
        double locsum = 0;
        double sum = 0;
        for (int i = my_rank; i < countintervals; i += num_procs) {
            sum += f(leftbound + i * step + step / 2);
        }
        locsum = step * sum;
        MPI_Reduce(&locsum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    } else {
        return step * f((rightbound + leftbound) / 2);
    }
    return result;
}
