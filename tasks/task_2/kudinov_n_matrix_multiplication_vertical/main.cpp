// Copyright 2023 Kudinov Nikita
#include <gtest/gtest.h>
#include <boost/mpi.hpp>

#include "task_2/kudinov_n_matrix_multiplication_vertical/matrix_multiplication_vertical.h"

#define ASSERT_EQ_VECTORS(vector1, vector2)        \
    do {                                           \
        ASSERT_EQ(vector1.size(), vector2.size()); \
                                                   \
        for (int i = 0; i < vector1.size(); ++i) { \
          ASSERT_EQ(vector1[i], vector2[i]);       \
        }                                          \
    } while (0)

void base_test(const Matrix<double>& matrix, const Vector<double>& vector, const Vector<double>& actual_result) {
    boost::mpi::communicator world;

    Vector<double> result_parallel = multiply_matrices_parallel(matrix, vector);

    if (world.rank() == 0) {
        Vector<double> result_sequential = multiply_matrices_sequential(matrix, vector);

        ASSERT_EQ_VECTORS(result_sequential, result_parallel);
        ASSERT_EQ_VECTORS(result_sequential, actual_result);
        ASSERT_EQ_VECTORS(result_parallel, actual_result);
    }
}

TEST(Matrix_Multiplication_Vertical, Small_Matrix) {
    base_test(
        {
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 }
        },
        { 2, 3, 4 },
        { 20, 47, 74 });
}

TEST(Matrix_Multiplication_Vertical, Medium_Matrix) {
    base_test(
        {
            { 114, 575, 438, 682, 508, 668 },
            { 157, 41, 40, 516, 693, 193 },
            { 470, 964, 857, 828, 708, 278 },
            { 608, 560, 579, 721, 408, 606 },
        },
        { 743, 62, 988, 19, 211, 853 },
        { 1243046, 479369, 1657948, 1675221 });
}


TEST(Matrix_Multiplication_Vertical, Big_Matrix) {
    base_test(
        {
            { 432, 3, 214, 247, 597, 438, 624, 591, 678, 906 },
            { 317, 944, 274, 887, 279, 22, 787, 156, 652, 210 },
            { 410, 618, 238, 469, 61, 269, 629, 195, 518, 223 },
            { 359, 209, 402, 105, 615, 443, 806, 372, 735, 491 },
            { 955, 967, 674, 294, 563, 474, 955, 464, 490, 642 },
            { 134, 982, 702, 299, 213, 29, 260, 551, 199, 517 },
            { 925, 37, 331, 332, 570, 780, 204, 519, 879, 809 },
            { 242, 152, 936, 10, 365, 923, 448, 978, 909, 356 },
            { 357, 98, 895, 782, 694, 51, 529, 869, 656, 294 },
            { 246, 819, 415, 347, 763, 216, 919, 320, 876, 282 }
        },
        { 543, 554, 236, 742, 372, 19, 750, 620, 709, 376 },
        { 2356200, 2750329, 2040731, 2261573, 3242669, 1956210, 2476268, 2317944, 2809922, 2845424 });
}

TEST(Matrix_Multiplication_Vertical, Empty_Matrix) {
    base_test(
        {},
        {},
        {});
}

TEST(Matrix_Multiplication_Vertical, Incompatible_Matrix_And_Vector) {
    try {
        base_test(
            {
                { 1, 2, 3, 4, 5 }
            },
            { 1, 2, 3 },
            {});
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& error) {
        EXPECT_EQ(
            error.what(),
            std::string("The number of columns in the matrix must be equal to the size of the vector"));
    } catch (...) {
        FAIL() << "Expected std::runtime_error";
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
