// Copyright 2023 Kruglov Alexey
#include <gtest/gtest.h>
#include "task_1/kruglov_a_number_of_orderly_violations/number_of_orderly_violations.h"


TEST(Num_Violation_Order_Vector, Test_Only_Gen_Procs) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector;
    const int Vector_len = 128;
    if (rank == 0) {
        Vector = generateRandomVector(Vector_len);
    }
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector_len);
    if (rank == 0) {
        int seqential_num = numberOfOrderValids_Sync(Vector);
        ASSERT_EQ(seqential_num, parellel_num);
    }
}

TEST(Num_Violation_Order_Vector, Test_Odd_Size_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector;
    const int Vector_len = 125;
    if (rank == 0) {
        Vector = generateRandomVector(Vector_len);
    }
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector_len);
    if (rank == 0) {
        int seqential_num = numberOfOrderValids_Sync(Vector);
        ASSERT_EQ(seqential_num, parellel_num);
    }
}

TEST(Num_Violation_Order_Vector, Test_Ordered_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector(20);
    if (rank == 0) {
        for (size_t i = 0; i < Vector.size(); ++i) {
            Vector[i] = i;
        }
    }
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector.size());
    if (rank == 0) {
        ASSERT_EQ(parellel_num, 0);
    }
}

TEST(Num_Violation_Order_Vector, Test_Const_Unordered_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector = { 9, 5, 2, 57, 2, 47, 1, 6, 3, 5, 2, 67, 2, 4, 73, 2, 5 };
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector.size());
    if (rank == 0) {
        ASSERT_EQ(parellel_num, 8);
    }
}

TEST(Num_Violation_Order_Vector, Test_Vector_Identical_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector(20, 5);
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector.size());
    if (rank == 0) {
        ASSERT_EQ(parellel_num, 0);
    }
}

TEST(Num_Violation_Order_Vector, Test_Sorted_In_Descending_Order_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector(20);
    if (rank == 0) {
        for (size_t i = 0; i < Vector.size(); ++i) {
            Vector[i] = Vector.size() - i - 1;
        }
    }
    int parellel_num = numberOfOrderValids_ASync(Vector, Vector.size());
    if (rank == 0) {
        ASSERT_EQ(parellel_num, Vector.size() - 1);
    }
}

TEST(Num_Violation_Order_Vector, Test_Big_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector;
    const int Vector_len = 100000;
    if (rank == 0) {
        Vector = generateRandomVector(Vector_len);
    }
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector_len);
    if (rank == 0) {
        int seqential_num = numberOfOrderValids_Sync(Vector);
        ASSERT_EQ(seqential_num, parellel_num);
    }
}

TEST(Num_Violation_Order_Vector, Test_Zero_Elements_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector;
    const int Vector_len = 0;
    if (rank == 0) {
        Vector = generateRandomVector(Vector_len);
    }
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector_len);
    if (rank == 0) {
        ASSERT_EQ(parellel_num, 0);
    }
}

TEST(Num_Violation_Order_Vector, Test_One_Elements_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vector;
    const int Vector_len = 1;
    if (rank == 0) {
        Vector = generateRandomVector(Vector_len);
    }
    int  parellel_num = numberOfOrderValids_ASync(Vector, Vector_len);
    if (rank == 0) {
        ASSERT_EQ(parellel_num, 0);
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
