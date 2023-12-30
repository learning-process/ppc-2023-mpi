// Copyright 2023 Gontsov Roman
#include <gtest/gtest.h>
#include <vector>
#include "./star.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Star_Topology, Test_Child_To_Child) {
    boost::mpi::communicator world;
    int data = 0;
    int root = 0;
    int sender = 1;
    int recipient = world.size() - 1;

    if (world.size() > 1) {
        if (world.rank() == sender) {
            data = 100;
        }

        int received_data = send_data(data, sender, recipient, root);

        if (world.rank() == recipient) {
            ASSERT_EQ(received_data, 100);
        }
    }
}

TEST(Star_Topology, Test_Root_To_Child) {
    boost::mpi::communicator world;
    int data = 0;
    int root = 0;
    int sender = 0;
    int recipient = world.size() - 1;

    if (world.rank() == sender) {
        data = 100;
    }

    int received_data = send_data(data, sender, recipient, root);

    if (world.rank() == recipient) {
        ASSERT_EQ(received_data, 100);
    }
}

TEST(Star_Topology, Test_Child_To_Root) {
    boost::mpi::communicator world;
    int data = 0;
    int root = 0;
    int sender = world.size() - 1;
    int recipient = 0;

    if (world.rank() == sender) {
        data = 100;
    }

    int received_data = send_data(data, sender, recipient, root);

    if (world.rank() == recipient) {
        ASSERT_EQ(received_data, 100);
    }
}

TEST(Star_Topology, Test_Child_To_Child_2) {
    boost::mpi::communicator world;
    int data = 0;
    int root = 0;
    int sender = 2;
    int recipient = 1;

    if (world.size() > 2) {
        if (world.rank() == sender) {
            data = 100;
        }

        int received_data = send_data(data, sender, recipient, root);

        if (world.rank() == recipient) {
            ASSERT_EQ(received_data, 100);
        }
    }
}

TEST(Star_Topology, Test_Sender_Is_Recipient) {
    boost::mpi::communicator world;
    int data = 0;
    int root = 0;
    int sender = world.size() - 1;
    int recipient = world.size() - 1;

    if (world.rank() == sender) {
        data = 100;
    }

    int received_data = send_data(data, sender, recipient, root);

    if (world.rank() == recipient) {
        ASSERT_EQ(received_data, 100);
    }
}

TEST(Star_Topology, Test_Sender_Is_Recipient_2) {
    boost::mpi::communicator world;
    int data = 0;
    int root = 0;
    int sender = root;
    int recipient = root;

    if (world.rank() == sender) {
        data = 100;
    }

    int received_data = send_data(data, sender, recipient, root);

    if (world.rank() == recipient) {
        ASSERT_EQ(received_data, 100);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
