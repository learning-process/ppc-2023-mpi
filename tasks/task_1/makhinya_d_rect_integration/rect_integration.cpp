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

    for( int i = 0; i < count_partitions; ++i) {
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

    double local_res = 0.0;
    if (world.rank() == 0) {
        auto sz = world.size();
        double dx_per_proc =  (b - a) / sz;

        for (int proc = 1; proc < sz; proc++) {
            double loc_a = a + proc * dx_per_proc;
            double loc_b = loc_a + dx_per_proc;
            bounds local_bounds = {loc_a, loc_b};
            world.send(proc, 0, local_bounds);
        }
        uint32_t delta = count_partitions / sz + count_partitions % world.size();
        double loc_a = a;
        double loc_b = a + dx_per_proc;
        local_res = getSequentialIntegral(func, loc_a, loc_b, delta);
    }


    if( world.rank() != 0) {
        bounds local_bounds;
        world.recv(0, 0, local_bounds);
        double loc_a = local_bounds[0];
        double loc_b = local_bounds[1];
        uint32_t delta = count_partitions / world.size();
        local_res = getSequentialIntegral(func, loc_a, loc_b, delta);
    }

    double global_res = 0.0;

    reduce(world, local_res, global_res, std::plus<double>(), 0);

    return global_res;
}
