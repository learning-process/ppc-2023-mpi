// Copyright 2023 Kiselev Igor
#include <gtest/gtest.h>
#include "./strassen_mul.h"

TEST(strassen_mul_test, test_2x2_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 2;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);

    std::vector<std::vector<double>> A = { { 1.0, 1.0 },
        { 1.0, 1.0 } };
    std::vector<std::vector<double>> B = { { 1.0, 1.0 },
        { 1.0, 1.0 } };
    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = SeqMulMatrix(A, B, N);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = Strassen(A, B, N);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);

    std::vector<std::vector<double>> A = { { 1.0, 1.0, 1.0, 1.0 },
       { 1.0, 1.0, 1.0, 1.0 },
       { 1.0, 1.0, 1.0, 1.0 },
       { 1.0, 1.0, 1.0, 1.0 } };
    std::vector<std::vector<double>> B = { { 1.0, 1.0, 1.0, 1.0 },
       { 1.0, 1.0, 1.0, 1.0 },
       { 1.0, 1.0, 1.0, 1.0 },
       { 1.0, 1.0, 1.0, 1.0 } };
    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = SeqMulMatrix(A, B, N);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = Strassen(A, B, N);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_progression) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);

    std::vector<std::vector<double>> A = { { 1.0, 2.0, 3.0, 4.0 },
       { 5.0, 6.0, 7.0, 8.0 },
       { 9.0, 10.0, 11.0, 12.0 },
       { 13.0, 14.0, 15.0, 16.0 } };
    std::vector<std::vector<double>> B = { { 1.0, 2.0, 3.0, 4.0 },
       { 5.0, 6.0, 7.0, 8.0 },
       { 9.0, 10.0, 11.0, 12.0 },
       { 13.0, 14.0, 15.0, 16.0 } };
    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = SeqMulMatrix(A, B, N);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = Strassen(A, B, N);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_random) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);

    std::vector<std::vector<double>> A = { { 34.0, 54.0, 21.0, 44.0 },
       { 23.0, 45.0, 21.0, 8.0 },
       { 78.0, 33.0, 42.0, 45.0 },
       { 63.0, 73.0, 12.0, 40.0 } };
    std::vector<std::vector<double>> B = { { 64.0, 44.0, 31.0, 35.0 },
       { 86.0, 23.0, 54.0, 67.0 },
       { 85.0, 14.0, 23.0, 54.0 },
       { 76.0, 91.0, 19.0, 34.0 } };

    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = SeqMulMatrix(A, B, N);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = Strassen(A, B, N);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_10x10_of_random) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);

    std::vector<std::vector<double>> A =
    { { 99.0, 60.0, 80.0, 23.0, 37.0, 31.0, 97.0, 15.0, 73.0, 46.0, 75.0, 40.0, 12.0, 10.0, 12.0, 18.0 },
    { 51.0, 35.0, 94.0, 31.0, 35.0, 81.0, 32.0, 74.0, 99.0, 94.0, 43.0, 32.0, 16.0, 28.0, 65.0, 90.0 },
    { 50.0, 21.0, 76.0, 82.0, 95.0, 83.0, 44.0, 98.0, 18.0, 60.0, 39.0, 18.0, 55.0, 4.0, 96.0, 90.0 },
    { 99.0, 28.0, 24.0, 11.0, 4.0, 50.0, 55.0, 6.0, 29.0, 61.0, 6.0, 81.0, 23.0, 80.0, 38.0, 38.0 },
    { 48.0, 48.0, 68.0, 96.0, 70.0, 54.0, 91.0, 73.0, 85.0, 64.0, 39.0, 18.0, 81.0, 32.0, 10.0, 56.0 },
    { 24.0, 48.0, 97.0, 31.0, 26.0, 5.0, 23.0, 10.0, 22.0, 23.0, 15.0, 14.0, 88.0, 90.0, 91.0, 77.0 },
    { 12.0, 31.0, 56.0, 28.0, 88.0, 96.0, 76.0, 14.0, 41.0, 71.0, 81.0, 52.0, 83.0, 99.0, 63.0, 62.0 },
    { 96.0, 30.0, 40.0, 64.0, 92.0, 90.0, 40.0, 69.0, 25.0, 64.0, 69.0, 87.0, 19.0, 64.0, 94.0, 90.0 },
    { 27.0, 20.0, 9.0, 23.0, 7.0, 61.0, 58.0, 74.0, 62.0, 64.0, 31.0, 94.0, 22.0, 58.0, 23.0, 90.0 },
    { 31.0, 87.0, 68.0, 95.0, 90.0, 65.0, 46.0, 33.0, 84.0, 89.0, 90.0, 75.0, 54.0, 72.0, 31.0, 59.0 },
    { 49.0, 75.0, 44.0, 1.0, 98.0, 5.0, 66.0, 10.0, 39.0, 55.0, 25.0, 70.0, 89.0, 61.0, 18.0, 65.0 },
    { 44.0, 13.0, 99.0, 34.0, 85.0, 45.0, 66.0, 82.0, 18.0, 96.0, 82.0, 72.0, 82.0, 65.0, 18.0, 57.0 },
    { 75.0, 61.0, 7.0, 32.0, 54.0, 24.0, 65.0, 26.0, 52.0, 17.0, 42.0, 0.0, 29.0, 39.0, 19.0, 55.0 },
    { 82.0, 68.0, 83.0, 88.0, 14.0, 23.0, 54.0, 65.0, 28.0, 28.0, 62.0, 93.0, 98.0, 30.0, 16.0, 12.0 },
    { 3.0, 16.0, 36.0, 20.0, 13.0, 6.0, 10.0, 20.0, 70.0, 23.0, 65.0, 71.0, 58.0, 96.0, 86.0, 52.0 },
    { 50.0, 22.0, 31.0, 49.0, 22.0, 78.0, 9.0, 8.0, 95.0, 93.0, 2.0, 7.0, 87.0, 19.0, 60.0, 11.0 } };
        std::vector<std::vector<double>> B =
    { { 74.0, 54.0, 0.0, 36.0, 76.0, 42.0, 78.0, 88.0, 16.0, 93.0, 36.0, 43.0, 12.0, 57.0, 85.0, 31.0 },
    { 65.0, 86.0, 94.0, 11.0, 49.0, 95.0, 77.0, 91.0, 10.0, 83.0, 96.0, 60.0, 43.0, 16.0, 40.0, 3.0 },
    { 15.0, 13.0, 31.0, 47.0, 15.0, 12.0, 57.0, 56.0, 28.0, 91.0, 94.0, 67.0, 30.0, 34.0, 8.0, 60.0 },
    { 92.0, 59.0, 11.0, 71.0, 68.0, 45.0, 88.0, 53.0, 26.0, 98.0, 55.0, 92.0, 0.0, 0.0, 78.0, 76.0 },
    { 82.0, 72.0, 46.0, 98.0, 68.0, 73.0, 21.0, 74.0, 14.0, 82.0, 25.0, 0.0, 94.0, 46.0, 37.0, 92.0 },
    { 75.0, 5.0, 67.0, 37.0, 88.0, 76.0, 1.0, 95.0, 56.0, 89.0, 60.0, 57.0, 25.0, 95.0, 64.0, 35.0 },
    { 55.0, 19.0, 41.0, 71.0, 54.0, 11.0, 63.0, 6.0, 44.0, 97.0, 31.0, 27.0, 2.0, 49.0, 60.0, 37.0 },
    { 44.0, 14.0, 95.0, 86.0, 25.0, 66.0, 31.0, 73.0, 16.0, 36.0, 42.0, 36.0, 72.0, 34.0, 57.0, 42.0 },
    { 44.0, 70.0, 26.0, 48.0, 7.0, 70.0, 55.0, 28.0, 44.0, 75.0, 31.0, 8.0, 56.0, 92.0, 2.0, 70.0 },
    { 38.0, 51.0, 78.0, 52.0, 23.0, 56.0, 27.0, 22.0, 33.0, 74.0, 83.0, 80.0, 58.0, 88.0, 15.0, 36.0 },
    { 2.0, 14.0, 53.0, 83.0, 15.0, 81.0, 47.0, 70.0, 81.0, 66.0, 1.0, 13.0, 8.0, 84.0, 95.0, 2.0 },
    { 85.0, 61.0, 5.0, 81.0, 18.0, 82.0, 36.0, 10.0, 53.0, 24.0, 33.0, 72.0, 57.0, 66.0, 91.0, 7.0 },
    { 83.0, 3.0, 61.0, 75.0, 53.0, 46.0, 33.0, 38.0, 53.0, 71.0, 92.0, 74.0, 87.0, 22.0, 65.0, 46.0 },
    { 30.0, 16.0, 64.0, 81.0, 41.0, 14.0, 3.0, 51.0, 6.0, 70.0, 89.0, 21.0, 85.0, 72.0, 54.0, 93.0 },
    { 49.0, 32.0, 8.0, 21.0, 37.0, 14.0, 18.0, 44.0, 46.0, 48.0, 93.0, 62.0, 78.0, 24.0, 89.0, 89.0 },
    { 43.0, 31.0, 11.0, 24.0, 5.0, 49.0, 19.0, 67.0, 90.0, 74.0, 41.0, 92.0, 14.0, 3.0, 32.0, 9.0 } };

    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = SeqMulMatrix(A, B, N);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = Strassen(A, B, N);
        ASSERT_EQ(CStr, CSeq);
    }
}

int main(int argc, char **argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);

    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    if (rankProc != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return 0;
}
