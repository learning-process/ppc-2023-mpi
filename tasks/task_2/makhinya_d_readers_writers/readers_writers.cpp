// Copyright 2023 Makhinya Danil
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_2/makhinya_d_readers_writers/readers_writers.h"

enum class READER_TAG {
    START = 1,
    READY = 2,
    END = 3
};

enum class WRITER_TAG {
    START = 4,
    READY = 5,
    END = 6
};

void run_problem_readers_writers(uint16_t readerCount, uint16_t writerCount) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int sizeWorld = world.size();

    if (rank == 0) {
        int in = 0;
        int out = 1;
        uint16_t processesLeft = std::min(static_cast<int>(readerCount + writerCount), (sizeWorld - 1));
        while (processesLeft) {
            boost::mpi::status status;
            auto flag = world.iprobe(boost::mpi::any_source, static_cast<int>(WRITER_TAG::READY));
            if (!flag.is_initialized()) {
                status = world.recv(boost::mpi::any_source, boost::mpi::any_tag, in);
            } else {
                status = world.recv(boost::mpi::any_source, static_cast<int>(WRITER_TAG::READY), in);
            }

            if (status.tag() == static_cast<int>(READER_TAG::READY)) {
                int in = 0;
                int out = 1;
                world.send(status.source(), static_cast<int>(READER_TAG::START), out);
                world.recv(status.source(), static_cast<int>(READER_TAG::END), in);
                processesLeft--;

            } else if (status.tag() == static_cast<int>(WRITER_TAG::READY)) {
                int in = 0;
                int out = 1;
                world.send(status.source(), static_cast<int>(WRITER_TAG::START), out);
                world.recv(status.source(), static_cast<int>(WRITER_TAG::END), in);
                processesLeft--;
            }
        }
    } else if (rank <= readerCount) {
        int in = 0;
        int out = 1;
        world.send(0, static_cast<int>(READER_TAG::READY), out);
        world.recv(0, static_cast<int>(READER_TAG::START), in);
        world.send(0, static_cast<int>(READER_TAG::END), out);
    } else if (rank <= readerCount + writerCount) {
        int in = 0;
        int out = 1;
        world.send(0, static_cast<int>(WRITER_TAG::READY), out);
        world.recv(0, static_cast<int>(WRITER_TAG::START), in);
        world.send(0, static_cast<int>(WRITER_TAG::END), out);
    }
}
