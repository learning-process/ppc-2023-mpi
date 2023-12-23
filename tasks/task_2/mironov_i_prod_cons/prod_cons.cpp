// Copyright 2023 Mironov Ilya
#include <iostream>
#include <numeric>
#include <random>
#include <boost/circular_buffer.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/group.hpp>
#include "task_2/mironov_i_prod_cons/prod_cons.h"

const int BUFF_SZ = 2;

const int BASE_PROC = 0;

const int READY_TAG = 0;
const int BLOCK_TAG = 1;
const int GO_TAG = 2;

int ProducerConsumer(int producers) {
    boost::mpi::communicator world;
    int ws = world.size(), wr = world.rank();
    int some_data = 42;
    boost::mpi::status status;
    int is_producer = wr <= producers;
    boost::mpi::communicator producers_comm = world.split(is_producer ? 0 : 1);
    boost::mpi::communicator consumers_comm = world.split(is_producer && wr != 0 ? 1 : 0);

    if (wr == 0) {
        int processes_to_service = ws - 1;
        int waiting_producers = 0, waiting_consumers = 0;
        boost::circular_buffer<int> cb(BUFF_SZ);
        while (processes_to_service) {
            if (waiting_producers) {
                boost::mpi::broadcast(producers_comm, some_data, BASE_PROC);
                waiting_producers--;
            } else if (waiting_consumers) {
                boost::mpi::broadcast(consumers_comm, some_data, BASE_PROC);
                waiting_consumers--;
            }
            status = world.recv(boost::mpi::any_source, READY_TAG, is_producer);
            if (is_producer) {
                if (cb.full()) {
                    world.send(status.source(), BLOCK_TAG);
                    waiting_producers++;
                } else {
                    world.send(status.source(), READY_TAG, some_data);  // fiction data
                    world.recv(status.source(), GO_TAG, some_data);
                    cb.push_back(some_data);
                    processes_to_service--;
                }
            } else {
                if (cb.size() == 0) {
                    world.send(status.source(), BLOCK_TAG);
                    waiting_consumers++;
                } else {
                    some_data = cb.back();
                    world.send(status.source(), READY_TAG, some_data);
                    cb.pop_back();
                    processes_to_service--;
                }
            }
        }
    } else {
        while (true) {
            world.send(BASE_PROC, READY_TAG, is_producer);
            status = world.recv(BASE_PROC, boost::mpi::any_tag, some_data);

            if (status.tag() == READY_TAG) {
                if (is_producer) {
                    some_data = std::pow(2, wr);
                    world.send(BASE_PROC, GO_TAG, some_data);
                }
                break;
            }

            if (is_producer) {
                boost::mpi::broadcast(producers_comm, some_data, BASE_PROC);
            } else {
                boost::mpi::broadcast(consumers_comm, some_data, BASE_PROC);
            }
        }
    }
    return 0;
}
