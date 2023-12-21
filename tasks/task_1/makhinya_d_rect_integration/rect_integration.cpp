// Copyright 2023 Makhinya Danil
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_1/makhinya_d_rect_integration/rect_integration.h"


double getSequentialIntegral(std::function<double(double)> func, 
                             double a, double b, uint32_t count_partitions) {
    double res = 0.0;
    const double dx = (b - a) / count_partitions;

    for(int i = 0; i < count_partitions; ++i) {
        double x = a + dx * i;
        double y = func(x);
        res += y * dx;
    }
    return res;
}
using bounds = std::array<double, 2>;
double getParallelIntegral(std::function<double(double)> func, 
                           double a, double b, uint32_t count_partitions) {
    boost::mpi::communicator world;

    bool is_invert_sign = false;
    if(a >= b) {
        std::swap(a, b);
        is_invert_sign = true;
    }

    int useless_ptr[5] = {0};
    if (world.rank() == 0) {
        int sz = world.size();
        bounds local_bounds = {a, b};
        for (int proc = 1; proc < world.size(); proc++) {
            world.send(proc, 0, local_bounds);
        }
    }

    double global_res = 0.0;
    double local_res = 0.0;

    if(world.rank() != 0)
    {
        bounds local_bound;
        world.recv(0, 0, local_bound);

        local_res = getSequentialIntegral(func, a, b, count_partitions);
        reduce(world, local_res, global_res, std::plus<int>(), 0.0);
    }

    if(is_invert_sign)
        global_res *= -1.0;

    return global_res;
}
