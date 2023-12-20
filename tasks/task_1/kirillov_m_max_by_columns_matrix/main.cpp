// Copyright 2023 Kirillov Maxim
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_column_max.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Matrix_Column_Max, Test_Max1){
    boost::mpi::communicator world;
    std::vector<int> matrix;
    const int rows = 4,
              columns = 3;
    if (world.rank() == 0){
         matrix = { 10, 4, 2, 5,
                    51, 23, 7, 10,
                    21, 0, 3, 9,
                    7, 17, 1, 31};
    }
    std::vector<int> resParallel = getParallelMaxInColumns(matrix, rows, columns);
    if (world.rank() == 0){
        std::vector<int> resSequential = getSequentialMaxInColumns(matrix, rows, columns);
        ASSERT_EQ(resParallel, resSequential);
    }
}

TEST(Matrix_Column_Max, Test_Max2){
    boost::mpi::communicator world;
    std::vector<int> matrix;
    const int rows = 3,
              columns = 4;
    if (world.rank() == 0){
        matrix = getRandomMatrix(rows, columns);
    }
    std::vector<int> resParallel = getParallelMaxInColumns(matrix, rows, columns);
    if (world.rank() == 0){
        std::vector<int> resSequential = getSequentialMaxInColumns(matrix, rows, columns);
        ASSERT_EQ(resParallel, resSequential);
    }
}

TEST(Matrix_Column_Max, Test_Max3){
    boost::mpi::communicator world;
    std::vector<int> matrix;
    const int rows = 10,
              columns = 10;
    if (world.rank() == 0){
        matrix = getRandomMatrix(rows, columns);
    }
    std::vector<int> resParallel = getParallelMaxInColumns(matrix, rows, columns);
    if (world.rank() == 0){
        std::vector<int> resSequential = getSequentialMaxInColumns(matrix, rows, columns);
        ASSERT_EQ(resParallel, resSequential);
    }
}


TEST(Matrix_Column_Max, Test_Max4){
    boost::mpi::communicator world;
    std::vector<int> matrix;
    const int rows = 1,
            columns = 1;
    if (world.rank() == 0){
        matrix = getRandomMatrix(rows, columns);
    }
    std::vector<int> resParallel = getParallelMaxInColumns(matrix, rows, columns);
    if (world.rank() == 0){
        std::vector<int> resSequential = getSequentialMaxInColumns(matrix, rows, columns);
        ASSERT_EQ(resParallel, resSequential);
    }
}

TEST(Matrix_Column_Max, Test_Max5){
    boost::mpi::communicator world;
    std::vector<int> matrix;
    const int rows = 7,
            columns = 9;
    if (world.rank() == 0){
        matrix = getRandomMatrix(rows, columns);
    }
    std::vector<int> resParallel = getParallelMaxInColumns(matrix, rows, columns);
    if (world.rank() == 0){
        std::vector<int> resSequential = getSequentialMaxInColumns(matrix, rows, columns);
        ASSERT_EQ(resParallel, resSequential);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
