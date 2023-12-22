// Copyright 2023 Ivanov Nikita
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include "task_3/ivanov_constr_conv_jarvis/constr_conv_jarvis.h"

TEST(Jarvis, test_1) {
    boost::mpi::communicator world;
    int n = 9;
    int m = 11;
    int delta = n / world.size();
    std::vector<std::vector<int>> image(0);
    std::vector<int> sizes;

    if (world.rank() == 0) {
        image.resize(n, std::vector<int> (m, 0));
        image = {
                {0,   0,   0,  0,   0, 0,   0,  0,   0,   0, 0},  // 0
                {0,   0, 100,  0,   0, 0, 245,  0,   0,   0, 0},  // 1
                {0,   0, 200,  0,   0, 0,   0,  0,   0,   0, 0},  // 2
                {0,   0,   0,  0, 241, 0,   0,  0, 251,   0, 0},  // 3
                {0,   0,   0, 45,   0, 0, 199,  0,   0,   0, 0},  // 4
                {0, 196,   0,  0,   0, 0,  52,  0,   0,   0, 0},  // 5
                {0,   0,   0,  0, 203, 0,   0,  0,   0, 211, 0},  // 6
                {0,   0,   0,  0,   0, 0,   0, 12,   0,   0, 0},  // 7
                {0,   0, 145,  0,   0, 0,   0,  0,  77,   0, 0}   // 8
        };
    }

    for (int i = 0; i < world.size() - 1; i++)
        sizes.emplace_back(delta);
    sizes.emplace_back(delta + n - world.size() * delta);

    if (world.rank() == world.size() - 1)
        delta += n - delta * world.size();

    std::vector<std::vector<int>> image_slice(delta, std::vector<int> (m));

    boost::mpi::scatterv(world, image.data(), sizes, image_slice.data(), 0);

    std::vector<P> points_J = JarvisParallel(image_slice, n);

    if (world.rank() == 0) {
        std::vector<std::pair<int, int>> points = get_points_from_image(image, n);
        bool all_inside_flag = inside_conv(points_J, points);
        ASSERT_EQ(all_inside_flag, true);
    }
}

TEST(Jarvis, test_2) {
    boost::mpi::communicator world;

    int n = 100;
    int m = 100;
    int delta = n / world.size();
    std::vector<std::vector<int>> image(0);
    std::vector<int> sizes;

    if (world.rank() == 0) {
        image.resize(n, std::vector<int> (m, 0));
        image = create_image(n, m);
    }

    for (int i = 0; i < world.size() - 1; i++)
        sizes.emplace_back(delta);
    sizes.emplace_back(delta + n - world.size() * delta);

    if (world.rank() == world.size() - 1)
        delta += n - delta * world.size();
    std::vector<std::vector<int>> image_slice(delta, std::vector<int> (m));
    boost::mpi::scatterv(world, image.data(), sizes, image_slice.data(), 0);

    std::vector<P> points_J = JarvisParallel(image_slice, n);

    if (world.rank() == 0) {
        std::vector<std::pair<int, int>> points = get_points_from_image(image, n);
        bool all_inside_flag = inside_conv(points_J, points);
        ASSERT_EQ(all_inside_flag, true);
    }
}

TEST(Jarvis, test_3) {
    boost::mpi::communicator world;

    int n = 200;
    int m = 300;
    int delta = n / world.size();
    std::vector<std::vector<int>> image(0);
    std::vector<int> sizes;

    if (world.rank() == 0) {
        image.resize(n, std::vector<int> (m, 0));
        image = create_image(n, m);
    }

    for (int i = 0; i < world.size() - 1; i++)
        sizes.emplace_back(delta);
    sizes.emplace_back(delta + n - world.size() * delta);

    if (world.rank() == world.size() - 1)
        delta += n - delta * world.size();
    std::vector<std::vector<int>> image_slice(delta, std::vector<int> (m));

    boost::mpi::scatterv(world, image.data(), sizes, image_slice.data(), 0);

    std::vector<P> points_J = JarvisParallel(image_slice, n);

    if (world.rank() == 0) {
        std::vector<std::pair<int, int>> points = get_points_from_image(image, n);
        bool all_inside_flag = inside_conv(points_J, points);
        ASSERT_EQ(all_inside_flag, true);
    }
}

TEST(Jarvis, test_4) {
    boost::mpi::communicator world;

    int n = 400;
    int m = 500;
    int delta = n / world.size();
    std::vector<std::vector<int>> image(0);
    std::vector<int> sizes;

    if (world.rank() == 0) {
        image.resize(n, std::vector<int> (m, 0));
        image = create_image(n, m);
    }

    for (int i = 0; i < world.size() - 1; i++)
        sizes.emplace_back(delta);
    sizes.emplace_back(delta + n - world.size() * delta);

    if (world.rank() == world.size() - 1)
        delta += n - delta * world.size();
    std::vector<std::vector<int>> image_slice(delta, std::vector<int> (m));

    boost::mpi::scatterv(world, image.data(), sizes, image_slice.data(), 0);

    std::vector<P> points_J = JarvisParallel(image_slice, n);

    if (world.rank() == 0) {
        std::vector<std::pair<int, int>> points = get_points_from_image(image, n);
        bool all_inside_flag = inside_conv(points_J, points);
        ASSERT_EQ(all_inside_flag, true);
    }
}

TEST(Jarvis, test_5) {
    boost::mpi::communicator world;

    int n = 500;
    int m = 600;
    int delta = n / world.size();
    std::vector<std::vector<int>> image(0);
    std::vector<int> sizes;

    if (world.rank() == 0) {
        image.resize(n, std::vector<int> (m, 0));
        image = create_image(n, m);
    }

    for (int i = 0; i < world.size() - 1; i++)
        sizes.emplace_back(delta);
    sizes.emplace_back(delta + n - world.size() * delta);

    if (world.rank() == world.size() - 1)
        delta += n - delta * world.size();
    std::vector<std::vector<int>> image_slice(delta, std::vector<int> (m));

    boost::mpi::scatterv(world, image.data(), sizes, image_slice.data(), 0);

    std::vector<P> points_J = JarvisParallel(image_slice, n);

    if (world.rank() == 0) {
        std::vector<std::pair<int, int>> points = get_points_from_image(image, n);
        bool all_inside_flag = inside_conv(points_J, points);
        ASSERT_EQ(all_inside_flag, true);
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
