// Copyright 2023 Makhinya Danil
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_3/makhinya_d_conjugate_gradient_method/gradient_method.h"

vector solve_parallel(const matrix& A, const vector& b)
{
    vector x(b.size());
    boost::mpi::communicator world;

    /*
        TODO: 
    */

    int *useless_ptr = nullptr;
    if (world.rank() == 0) {
        world.send(0, 0, useless_ptr, 0);
    }

    return x;
}