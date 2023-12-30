// Copyright 2023 Kochetov Nikolay
#include <mpi.h>
#include <gtest/gtest.h>
#include "task_3/kochetov_n_conv_hull/conv_hull.h"

TEST(TestConvHullMPI, Test_Empty) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<point> ans;
    std::vector<std::vector<point>> components;

    if (ProcRank == 0) {
        int size = 8;
        int image[] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };
        components = marking(image, size);
        ans = find_hull(components);
    } else {
        find_hull(components);
    }

    if (ProcRank == 0) {
        ASSERT_EQ(ans.size(), 0);
    }
}

TEST(TestConvHullMPI, Test_Frame) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<point> ans;
    std::vector<std::vector<point>> components;
    std::vector<point> correct_ans {
        point{0, 0}, point{7, 0}, point{7, 7}, point{0, 7}};

    if (ProcRank == 0) {
        int size = 8;
        int image[] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 1, 0, 1, 1, 0, 1,
            1, 0, 1, 0, 0, 0, 0, 1,
            1, 0, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 1, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
        };
        components = marking(image, size);
        ans = find_hull(components);
    } else {
        find_hull(components);
    }

    if (ProcRank == 0) {
        bool correct = (ans.size() == correct_ans.size());
        for (int i = 0; i < ans.size(); i++) {
            if (ans[i] != correct_ans[i]) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(TestConvHullMPI, Test_Circle) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<point> ans;
    std::vector<std::vector<point>> components;
    std::vector<point> correct_ans {
        point{0, 3}, point{1, 1}, point{3, 0}, point{4, 0},
        point{6, 1}, point{7, 3}, point{7, 4}, point{6, 6},
        point{4, 7}, point{3, 7}, point{1, 6}, point{0, 4}};

    if (ProcRank == 0) {
        int size = 8;
        int image[] = {
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 0, 1, 1, 0,
            0, 1, 0, 1, 0, 0, 1, 0,
            1, 0, 1, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 1, 0, 1,
            0, 1, 0, 0, 1, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
        };
        components = marking(image, size);
        ans = find_hull(components);
    } else {
        find_hull(components);
    }

    if (ProcRank == 0) {
        bool correct = (ans.size() == correct_ans.size());
        for (int i = 0; i < ans.size(); i++) {
            if (ans[i] != correct_ans[i]) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(TestConvHullMPI, Test_Checkerboard) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<point> ans;
    std::vector<std::vector<point>> components;
    std::vector<point> correct_ans {
        point{0, 0}, point{6, 0}, point{7, 1},
        point{7, 7}, point{1, 7}, point{0, 6}
        };

    if (ProcRank == 0) {
        int size = 8;
        int image[] = {
            1, 0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0, 1,
        };
        components = marking(image, size);
        ans = find_hull(components);
    } else {
        find_hull(components);
    }

    if (ProcRank == 0) {
        bool correct = (ans.size() == correct_ans.size());
        for (int i = 0; i < ans.size(); i++) {
            if (ans[i] != correct_ans[i]) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(TestConvHullMPI, Test_Random) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<point> ans;
    std::vector<std::vector<point>> components;
    std::vector<point> correct_ans {
        point{0, 3}, point{1, 1}, point{5, 1}, point{6, 2},
        point{6, 3}, point{3, 7}, point{1, 7}, point{0, 6}};

    if (ProcRank == 0) {
        int size = 8;
        int image[] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 1, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 0,
            1, 0, 0, 0, 1, 1, 1, 0,
            0, 0, 1, 1, 0, 1, 0, 0,
            1, 0, 0, 1, 0, 0, 0, 0,
            1, 1, 1, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 0, 0, 0, 0,
        };
        components = marking(image, size);
        ans = find_hull(components);
    } else {
        find_hull(components);
    }

    if (ProcRank == 0) {
        bool correct = (ans.size() == correct_ans.size());
        for (int i = 0; i < ans.size(); i++) {
            if (ans[i] != correct_ans[i]) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (ProcRank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
