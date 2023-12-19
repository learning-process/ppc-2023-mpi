// Copyright 2023 Simonyan Suren
#include <gtest/gtest.h>
#include <math.h>
#include "task_2/simonyan_s_allreduce/Allreduce.h"
#define ESTIMATE 0.0001

TEST(Allreduce, test_MPI_Double) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double value = 10.5;
    double sum = 0;
    double* another = new double[world_size];
    int root = 0;
    double t1 = MPI_Wtime();
    int return_code = My_AllReduce(&value, &sum, 1,
        MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    double t2 = MPI_Wtime();
    double t3 = MPI_Wtime();
    return_code = MPI_Allreduce(&value, &sum, 1,
        MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    double t4 = MPI_Wtime();


    if (rank != root) {
        MPI_Send(&sum, 1, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
        for (int i = 0; i < world_size; ++i) {
            if (i != root) {
                MPI_Recv(another + i, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        another[root] = sum;
    }
    if (return_code == MPI_SUCCESS && rank == root) {
        double min = another[0];
        double max = another[0];
        for (int i = 0; i < world_size - 1; ++i) {
            min = std::min(another[i], another[i + 1]);
            max = std::max(another[i], another[i + 1]);
        }
        ASSERT_EQ(min == max, sum == value * world_size);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Allreduce, test_MPI_Float) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    float value = 10.5;
    float sum = 0;
    float* another = new float[world_size];
    int root = 0;
    int return_code = My_AllReduce(&value, &sum, 1,
        MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank != root) {
        MPI_Send(&sum, 1, MPI_FLOAT, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
        for (int i = 0; i < world_size; ++i) {
            if (i != root) {
                MPI_Recv(another + i, 1, MPI_FLOAT, MPI_ANY_SOURCE,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        another[root] = sum;
    }
    if (return_code == MPI_SUCCESS && rank == root) {
        float min = another[0];
        float max = another[0];
        for (int i = 0; i < world_size - 1; ++i) {
            min = std::min(another[i], another[i + 1]);
            max = std::max(another[i], another[i + 1]);
        }
        ASSERT_EQ(min == max, sum == value * world_size);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Allreduce, test_MPI_Int) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int value = 10;
    int sum = 0;
    int* another = new int[world_size];
    int root = 0;
    int return_code = My_AllReduce(&value, &sum, 1,
        MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank != root) {
        MPI_Send(&sum, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
        for (int i = 0; i < world_size; ++i) {
            if (i != root) {
                MPI_Recv(another + i, 1, MPI_INT, MPI_ANY_SOURCE,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        another[root] = sum;
    }
    if (return_code == MPI_SUCCESS && rank == root) {
        double min = another[0];
        double max = another[0];
        for (int i = 0; i < world_size - 1; ++i) {
            min = std::min(another[i], another[i + 1]);
            max = std::max(another[i], another[i + 1]);
        }
        ASSERT_EQ(min == max, sum == value * world_size);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Allreduce, test_Multiplycate) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double value = 5;
    double multi = 0;
    double* another = new double[world_size];
    int root = 0;
    int return_code = My_AllReduce(&value, &multi, 1,
        MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);
    if (rank != root) {
        MPI_Send(&multi, 1, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
        for (int i = 0; i < world_size; ++i) {
            if (i != root) {
                MPI_Recv(another + i, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        another[root] = multi;
    }
    if (return_code == MPI_SUCCESS && rank == root) {
        double min = another[0];
        double max = another[0];
        for (int i = 0; i < world_size - 1; ++i) {
            min = std::min(another[i], another[i + 1]);
            max = std::max(another[i], another[i + 1]);
        }
        ASSERT_EQ(min == max, multi == pow(value, world_size));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}


TEST(Allreduce, test_time) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double value = 10.0;
    double sum = 0;
    int root = 0;
    double t1 = MPI_Wtime();
    int return_code = My_AllReduce(&value, &sum, 1,
        MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    double t2 = MPI_Wtime();
    double t3 = MPI_Wtime();
    return_code = MPI_Allreduce(&value, &sum, 1,
        MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    double t4 = MPI_Wtime();
    if (return_code == MPI_SUCCESS && rank == root) {
        ASSERT_LE(std::abs((t2 - t1)- (t4 - t3)), ESTIMATE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}


int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
