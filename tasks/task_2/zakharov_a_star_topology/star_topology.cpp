// Copyright 2023 Zakharov Artem
#include <boost/mpi/communicator.hpp>
#include "task_2/zakharov_a_star_topology/star_topology.h"


int send_data(const int& data, int sender, int recipient, int root) {
    boost::mpi::communicator comm;
    int local_data = -1;

    if (sender == recipient || sender >= comm.size() ||
        recipient >= comm.size() || root >= comm.size()) {
        return data;
    }

    if (comm.rank() == root) {
        if (root != sender) {
            comm.recv(sender, 0, local_data);
        } else {
            local_data = data;
        }

        if (root != recipient) {
            comm.send(recipient, 0, local_data);
        }
    } else if (comm.rank() == sender) {
        comm.send(root, 0, data);
        local_data = data;
    } else if (comm.rank() == recipient) {
        comm.recv(root, 0, local_data);
    }
    return local_data;
}
