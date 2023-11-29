// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./max_across_columns.h"

TEST(MAX_ACROSS_COLUMNS_MPI, test_sequential) {
    boost::mpi::communicator world;
    std::vector<int> global_matrix = {1, 6, 7, 12,
                                      2, 5, 8, 11,
                                      3, 4, 9, 10};
    const int rows = 3, columns = 4;
    std::vector<int> expected = {3, 6, 9, 12};
    std::vector<int> seqResult;
    if (world.rank() == 0) {
        seqResult = getSequentialOperations(global_matrix, rows, columns);
        ASSERT_EQ(expected, seqResult);
    }
}


int main(int argc, char** argv) {
    std::cout<<"Whatsup NIGGA\n";
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        std::cout<<"I WANNA BE YOUR SLAVE\n";
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
