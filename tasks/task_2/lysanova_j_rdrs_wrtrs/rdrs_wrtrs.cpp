// Copyright 2023 Lysanova Julia
#include <utility>
#include <unordered_map>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/lysanova_j_rdrs_wrtrs/rdrs_wrtrs.h"

#define SUPERVIZOR 0
enum TAGS {
    READY,
    DATA,
    DONE
};

enum ROLES {
    READER,
    WRITER
};


std::pair<std::queue<std::pair<int, int>>, std::vector<int>> rdrs_wrtrs_boost() {
    boost::mpi::communicator world;
    int shared_data = 0, role;

    if (world.rank() == 0) {
        std::unordered_map<int, boost::mpi::request> working_readers;
        std::queue<int> ready_writers;
        std::queue<std::pair<int, int>> serve_order;
        boost::mpi::status status;
        int process = world.size() - 1, readers_count = 0;
        while (process) {
            for (auto &i : working_readers) {
                if (i.second.test()) {
                    serve_order.push({i.first, READER});
                    readers_count--;
                    process--;
                    working_readers.erase(i.first);
                }
            }
            if (readers_count == 0 && ready_writers.size() > 0) {
                world.recv(ready_writers.front(), DATA, shared_data);
                serve_order.push({ready_writers.front(), WRITER});
                ready_writers.pop();
                process--;
            }
            if (world.iprobe(MPI_ANY_SOURCE, READY).has_value()) {
                status = world.recv(MPI_ANY_SOURCE, READY, role);
                if (role == WRITER) {
                    if (readers_count) {
                        ready_writers.push(status.source());
                    } else {
                        world.recv(status.source(), DATA, shared_data);
                        serve_order.push({status.source(), WRITER});
                        process--;
                    }
                } else {
                    readers_count++;
                    world.send(status.source(), DATA, shared_data);
                    working_readers[status.source()] = world.irecv(status.source(), DONE);
                }
            }
        }


        std::vector<int> datas(world.size());
        boost::mpi::gather(world, shared_data, datas, SUPERVIZOR);
        datas[0] = shared_data;
        world.barrier();
        return {serve_order, datas};
    } else {
        role = world.rank() % 2;
        world.send(SUPERVIZOR, READY, role);
        if (role == WRITER) {
            shared_data = world.rank() * 10;
            world.send(SUPERVIZOR, DATA, shared_data);
        } else {
            world.recv(SUPERVIZOR, DATA, shared_data);
            world.send(SUPERVIZOR, DONE);
        }
        boost::mpi::gather(world, shared_data, SUPERVIZOR);
        world.barrier();
        return {{}, {}};
    }
}
