// Copyright 2023 Volodin Evgeniy
#include <gtest/gtest.h>
#include "tasks/task_3/volodin_e_convex_hull_binary_image/chbi.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Operations_MPI, Rectangular_Image) {
    boost::mpi::communicator world;
    const int width = 8, height = 5;
    std::vector<std::vector<int>> image({
        {0, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 1, 1}
    });

    std::vector<int> convex_hull_par = getConvexHullPar(image, width, height);

    if (world.rank() == 0) {
        std::vector<int> convex_hull_seq = getConvexHullSeq(image, width, height);

        for (int i = 0; i < convex_hull_par.size(); ++i) {
            ASSERT_EQ(convex_hull_par[i], convex_hull_seq[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Square_Image) {
    boost::mpi::communicator world;
    const int width = 10, height = 10;
    std::vector<std::vector<int>> image = {
        {1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 1, 1, 1},
    };

    std::vector<int> convex_hull_par = getConvexHullPar(image, width, height);

    if (world.rank() == 0) {
        std::vector<int> convex_hull_seq = getConvexHullSeq(image, width, height);

        for (int i = 0; i < convex_hull_par.size(); ++i) {
            ASSERT_EQ(convex_hull_par[i], convex_hull_seq[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Random_Square_Image) {
    boost::mpi::communicator world;
    const int width = 12, height = 12;
    std::vector<std::vector<int>> image(height, std::vector<int>(width, 0));

    if (world.rank() == 0) {
        fillImageRandom(&image, width, height);
    }

    std::vector<int> convex_hull_par = getConvexHullPar(image, width, height);

    if (world.rank() == 0) {
        std::vector<int> convex_hull_seq = getConvexHullSeq(image, width, height);

        for (int i = 0; i < convex_hull_par.size(); ++i) {
            ASSERT_EQ(convex_hull_par[i], convex_hull_seq[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Random_Rectangular_Image_Small) {
    boost::mpi::communicator world;
    const int width = 7, height = 8;
    std::vector<std::vector<int>> image(height, std::vector<int>(width, 0));

    if (world.rank() == 0) {
        fillImageRandom(&image, width, height);
    }

    std::vector<int> convex_hull_par = getConvexHullPar(image, width, height);

    if (world.rank() == 0) {
        std::vector<int> convex_hull_seq = getConvexHullSeq(image, width, height);

        for (int i = 0; i < convex_hull_par.size(); ++i) {
            ASSERT_EQ(convex_hull_par[i], convex_hull_seq[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Random_Rectangular_Image_Big) {
    boost::mpi::communicator world;
    const int width = 35, height = 20;
    std::vector<std::vector<int>> image(height, std::vector<int>(width, 0));

    if (world.rank() == 0) {
        fillImageRandom(&image, width, height);
    }

    std::vector<int> convex_hull_par = getConvexHullPar(image, width, height);

    if (world.rank() == 0) {
        std::vector<int> convex_hull_seq = getConvexHullSeq(image, width, height);

        for (int i = 0; i < convex_hull_par.size(); ++i) {
            ASSERT_EQ(convex_hull_par[i], convex_hull_seq[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    RUN_ALL_TESTS();
    return 0;
}
