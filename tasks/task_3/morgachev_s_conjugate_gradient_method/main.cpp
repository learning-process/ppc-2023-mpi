// Copyright 2023 Morgachev Stepan
#include <gtest/gtest.h>
#include "./conjugate_gradient_method.h"

TEST(Conjugate_Gradient_Method, Test_1) {

}

TEST(Conjugate_Gradient_Method, Test_2) {

}

TEST(Conjugate_Gradient_Method, Test_3) {

}

TEST(Conjugate_Gradient_Method, Test_4) {

}

TEST(Conjugate_Gradient_Method, Test_5) {

}

int main(int argc, char** argv) {
    /*int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;*/
    size_t size = 3;
    std::vector<double> testMatrix = fillMatrixRandomNumbers(size, 1, 10);
    std::vector<double> testVector = fillVectorRandomNumbers(size, 1, 10);
    std::vector<double> res = serialConjugateGradient(testMatrix, testVector, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << testMatrix[i * size + j] << " ";
        }
        std::cout << std::endl;
    }

    for (double d : testVector) {
        std::cout << d << " ";
    }

    std::cout << std::endl;

    for (double d : res) {
        std::cout << d << " ";
    }

    std::cout << std::endl;
}
