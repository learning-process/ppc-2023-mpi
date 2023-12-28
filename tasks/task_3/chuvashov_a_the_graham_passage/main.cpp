// Copyright 2023 Chuvashov Andrey

#include <gtest/gtest.h>
#include "../tasks/task_3/chuvashov_a_the_graham_passage/the_graham_passage.h"

TEST(Grahams_passage, test_1) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    if (procId == 0) {
        ASSERT_ANY_THROW(GeneratePoints(-100));
    }
}

TEST(Grahams_passage, test_2) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<Point> Points{ {1, 4}, {0, 0}, {0, 2}, {1, 2}, {1, 0.5},
                               {1, 3}, {-2, 1}, {2, 0}, {2, 2}, {1, 5} };
    std::vector<Point> P = SortPoints(Points);
    std::vector<Point> paralRes = GrahamsPasParal(P);
    std::vector<Point> check{ {0, 0}, {2, 0}, {2, 2}, {1, 5}, {-2, 1} };
    bool a = 1;
    if (procId == 0) {
        for (size_t i = 0; i < paralRes.size(); i++)
            if (paralRes[i] != check[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

TEST(Grahams_passage, test_3) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<Point> P = GeneratePoints(100);
    std::vector<Point> paralRes = GrahamsPasParal(P);
    bool a = 1;
    if (procId == 0) {
        std::vector<Point> nonParalRes = GrahamsPasOneProc(P);
        for (size_t i = 0; i < paralRes.size(); i++)
            if (paralRes[i] != nonParalRes[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

TEST(Grahams_passage, test_4) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<Point> P = GeneratePoints(200);
    std::vector<Point>  paralRes = GrahamsPasParal(P);
    bool a = 1;
    if (procId == 0) {
        std::vector<Point> nonParalRes = GrahamsPasOneProc(P);
        for (size_t i = 0; i < paralRes.size(); i++)
            if (paralRes[i] != nonParalRes[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

TEST(Grahams_passage, test_5) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<Point> P = GeneratePoints(300);
    std::vector<Point>  paralRes = GrahamsPasParal(P);
    bool a = 1;
    if (procId == 0) {
        std::vector<Point> nonParalRes = GrahamsPasOneProc(P);
        for (size_t i = 0; i < paralRes.size(); i++)
            if (paralRes[i] != nonParalRes[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
