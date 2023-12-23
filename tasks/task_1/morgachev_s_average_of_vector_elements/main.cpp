// Copyright 2023 Morgachev Stepan
#include <gtest/gtest.h>
#include "./average_of_vector_elements.h"

TEST(Average_Of_Vector_Elements, Test_only_positive_data) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vector{ 11, 8, 3, 9 , 23, 2, 4, 30, 10, 41};

    double result = calculateAverageOfVectorElements(vector);

    if (rank == 0) {
        double resSeq = calculatePartialSum(vector) /
            static_cast<double> (vector.size());
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Average_Of_Vector_Elements, Test_only_negative_data) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vector{ -11, -8, -3, -9 , -23, -2, -4, -30, -10, -41 };

    double result = calculateAverageOfVectorElements(vector);

    if (rank == 0) {
        double resSeq = calculatePartialSum(vector) /
            static_cast<double> (vector.size());
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Average_Of_Vector_Elements, Test_only_one_number) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vector{ 1};

    double result = calculateAverageOfVectorElements(vector);

    if (rank == 0) {
        double resSeq = calculatePartialSum(vector) /
            static_cast<double> (vector.size());
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Average_Of_Vector_Elements, Test_only_zero) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vector{ 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0 };

    double result = calculateAverageOfVectorElements(vector);

    if (rank == 0) {
        double resSeq = calculatePartialSum(vector) /
            static_cast<double> (vector.size());
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Average_Of_Vector_Elements, Test_random) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t sizeVector = 1000000;
    const int lowerBound = -100000;
    const int upperBound = 100000;

    std::vector<int> vector(fillVectorRandomNumbers(sizeVector,
        lowerBound,
        upperBound));
    double result_par = calculateAverageOfVectorElements(vector);

    if (rank == 0) {
        double result_seq = calculatePartialSum(vector) /
            static_cast<double> (vector.size());
        ASSERT_EQ(result_par, result_seq);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
