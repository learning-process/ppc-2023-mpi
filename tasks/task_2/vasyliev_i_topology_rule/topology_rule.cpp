// Copyright 2023 Vasyliev Ivan
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/vasyliev_i_topology_rule/topology_rule.h"


void RULE_SEND(const boost::mpi::communicator& world, int val, int dst, int tag) {
    int rank = world.rank();
    if (rank > dst) world.send(rank - 1, tag, val);
    else            world.send(rank + 1, tag, val);
}

void RULE_RECV(const boost::mpi::communicator& world, const int &val, int src, int tag) {
    int rank = world.rank();
    if (rank > src) world.recv(rank - 1, tag, val);
    else            world.recv(rank + 1, tag, val);
}

void RULE_HELP(const boost::mpi::communicator& world, int src, int dst, int tag) {
    int rank = world.rank(), val, transport_tag = 8080, recv_tag, send_tag;
    recv_tag = send_tag = transport_tag;
    bool to_right = dst > src;
    if (to_right && rank < dst) {
        if (rank - 1 == src) recv_tag = tag;
        if (rank + 1 == dst) send_tag = tag;
        world.recv(rank - 1, recv_tag, val);
        world.send(rank + 1, send_tag, val);
    } else if (!to_right && rank > dst) {
        if (rank + 1 == src) recv_tag = tag;
        if (rank - 1 == dst) send_tag = tag;
        world.recv(rank + 1, recv_tag, val);
        world.send(rank - 1, send_tag, val);
    }
}
