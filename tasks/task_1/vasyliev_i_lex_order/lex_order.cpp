// Copyright 2023 Vasyliev Ivan
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/vasyliev_i_lex_order/lex_order.h"

int parallel_lex_cmp_(const char* str1, const char* str2, int len) {
    boost::mpi::communicator world;
    int loc_len = len / world.size();
    int remainder = len % world.size();
    std::vector<int> sizes(world.size(), loc_len);
    for (int i = 0; i < remainder; i++) { sizes[i]++; }
    char* local_str1 = new char[sizes[world.rank()] +1];
    char* local_str2 = new char[sizes[world.rank()] +1];

    boost::mpi::scatterv(world, str1, sizes, local_str1, 0);
    boost::mpi::scatterv(world, str2, sizes, local_str2, 0);

    local_str1[sizes[world.rank()]] = '\0';
    local_str2[sizes[world.rank()]] = '\0';

    int loc_cmp = 0;
    loc_cmp = strcmp(local_str1, local_str2);
    
    delete[] local_str1;
    delete[] local_str2;

    std::vector<int> cmps;
    if (world.rank() == 0) {
        cmps = std::vector<int>(world.size(), loc_cmp);
    }

    boost::mpi::gather(world, loc_cmp, cmps, 0);
    if (world.rank() == 0) {
        for (auto cmp : cmps) {
            if (cmp) {
                return cmp;
            }
        }
    }
    return 0;
}

int parallel_lex_cmp(const char* str1, const char* str2, int len1, int len2) {
    if (len1 != len2) {
        return (len1 - len2);
    } else {
        return parallel_lex_cmp_(str1, str2, len1);
    }
}
