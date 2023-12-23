// Copyright 2023 Kruglov Alexey
#include <gtest/gtest.h>
#include "task_2/kruglov_a_allreduce/allreduce.h"

static inline int MPIOverAllreduce(int rank, const void* send_buf, void* recv_buf,
    int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    double start, end;
    if (rank == 0) { start = MPI_Wtime(); }
    int res = MPI_Allreduce(send_buf, recv_buf, count, datatype, op, comm);
    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "MPIAllreduce time = " << end - start << std::endl;
    }
    return res;
}

static inline int myOverAllreduce(int rank, const void* send_buf, void* recv_buf,
    int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    double start, end;
    if (rank == 0) { start = MPI_Wtime(); }
    int res = myAllreduce(send_buf, recv_buf, count, datatype, op, comm);
    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "myAllreduce time = " << end - start << std::endl;
    }
    return res;
}

TEST(Parallel_Operations_MPI, Test_Operations_Correct) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> Vector(size);
    for (int i = 0; i < size; ++i) {
        Vector[i] = (i + rank) % size + 1;
    }

    std::vector<int> ARVector(size);
    std::vector<int> myRVector(size);

    MPI_Allreduce(&Vector[0], &ARVector[0], size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ((size + 1) * size / 2, myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_Int) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 256;

    std::vector<int> Vector = getRandomVector(vector_len);

    std::vector<int> ARVector(vector_len);
    std::vector<int> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(ARVector[0], myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_Float) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 256;

    std::vector<float> Vector = getRandomFloatVector(vector_len);

    std::vector<float> ARVector(vector_len);
    std::vector<float> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_LE(abs(ARVector[0] - myRVector[0]), 1e-5);
    }
}

TEST(Parallel_Operations_MPI, Test_Double) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 256;

    std::vector<double> Vector = getRandomDoubleVector(vector_len);

    std::vector<double> ARVector(vector_len);
    std::vector<double> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_LE(abs(ARVector[0] - myRVector[0]), 1e-9);
    }
}

TEST(Parallel_Operations_MPI, Test_MPI_Max) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 256;

    std::vector<int> Vector = getRandomVector(vector_len);

    std::vector<int> ARVector(vector_len);
    std::vector<int> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(ARVector[0], myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_MPI_Min) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 256;

    std::vector<double> Vector = getRandomDoubleVector(vector_len);

    std::vector<double> ARVector(vector_len);
    std::vector<double> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(ARVector[0], myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_MPI_Prod) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 25;

    std::vector<int> Vector = getRandomVector(vector_len);

    std::vector<int> ARVector(vector_len);
    std::vector<int> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(ARVector[0], myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_Big_Vector) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 10000;

    std::vector<int> Vector = getRandomVector(vector_len);

    std::vector<int> ARVector(vector_len);
    std::vector<int> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(ARVector[0], myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_Length_One_Vector) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 1;

    std::vector<int> Vector{ rank };

    std::vector<int> ARVector(vector_len);
    std::vector<int> myRVector(vector_len);

    MPI_Allreduce(&Vector[0], &ARVector[0], vector_len, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(ARVector[0], myRVector[0]);
    }
}

TEST(Parallel_Operations_MPI, Test_Math_Problem_Normalize) {
    // normalize vectors in random matrix sizeof: (MPI_size * vector_len), where metric(X) == sum(x[i])
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector_len = 10;  // Num of matrix columns, num of matrix rows == size
    std::vector<float> Vector = getRandomFloatVector(vector_len);
    std::vector<float> sumVector(vector_len);
    std::vector<float> myRVector(vector_len);

    myAllreduce(&Vector[0], &myRVector[0], vector_len, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < vector_len; ++i) {
        Vector[i] /= myRVector[i];
    }

    MPI_Reduce(&Vector[0], &sumVector[0], vector_len, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        float sum = 0;
        for (int i = 0; i < vector_len; ++i) {
            sum += sumVector[i];
        }
        ASSERT_LE(sum - vector_len, 1e-5);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int exec_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec_code;
}
