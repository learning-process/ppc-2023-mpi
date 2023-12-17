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

double getParallelIntegral(std::function<double(double)> func, 
                           double a, double b, uint32_t count_partitions) {
    boost::mpi::communicator world;

    /*
        TODO: 
    */

    int useless_ptr[5] = {0};
    if (world.rank() == 0) {
        world.send(0, 0, useless_ptr, 0);
    }

    return 0.0;
}
