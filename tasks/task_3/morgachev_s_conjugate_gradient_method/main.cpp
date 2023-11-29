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
