// Copyright 2023 Ivanov Nikita
#include <iostream>
#include <random>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include "task_3/ivanov_constr_conv_jarvis/constr_conv_jarvis.h"

int distance(std::pair<int, int> a, std::pair<int, int> b, std::pair<int, int> c) {
    int y1 = a.second - b.second;
    int y2 = a.second - c.second;
    int x1 = a.first - b.first;
    int x2 = a.first - c.first;
    int item1 = (y1*y1 + x1*x1);
    int item2 = (y2*y2 + x2*x2);
    if (item1 == item2)
        return 0;  // when b and c are in same distance from a
    else if (item1 < item2)
        return -1;  // when b is closer to a
    return 1;  // when c is closer to a
}

int crossProduct(std::pair<int, int> a, std::pair<int, int> b, std::pair<int, int> c) {
    int y1 = a.second - b.second;
    int y2 = a.second - c.second;
    int x1 = a.first - b.first;
    int x2 = a.first - c.first;
    return y2*x1 - y1*x2;
}

std::vector<std::pair<int, int>> get_points_from_image(const std::vector<std::vector<int>> &image, int n) {
    boost::mpi::communicator world;

    int rank = world.rank();
    int delta = n / world.size();


    std::vector<std::pair<int, int>> points;
    for (int i = 0; i < image.size(); i++)
        for (int j = 0; j < image[0].size(); j++)
            if (image[i][j] > 178)
                points.emplace_back(j, rank * delta + i);
    return points;
}

std::vector<P> JarvisParallel(const std::vector<std::vector<int>> &image, int n) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int commsize = world.size();

    std::vector<P> result;
    std::vector<std::pair<int, int>> selected_points(commsize, std::make_pair(image[0].size(), image[0].size()));
    std::pair<int, int> start_point = std::make_pair(image[0].size(), image[0].size());

    std::vector<std::pair<int, int>> points = get_points_from_image(image, n);
    for (auto p : points)
        if (p.second < start_point.second || (p.second == start_point.second && p.first < start_point.first))
            start_point = p;

    boost::mpi::gather(world, start_point, selected_points.data(), 0);
    if (rank == 0) {
        for (auto p : selected_points)
            if (p.second < start_point.second || (p.second == start_point.second && p.first < start_point.first))
                start_point = p;
    }
    boost::mpi::broadcast(world, start_point, 0);

    if (points.empty())
        points.emplace_back(start_point);

    std::pair<int, int> current = start_point;
    std::pair<int, int> next_point = points[0];

    while (true) {
        for (auto p : points) {
            if (p == current)
                continue;
            int val = crossProduct(current, next_point, p);
            if (val > 0) {
                next_point = p;
            } else if (val == 0) {
                if (distance(current, next_point, p) < 0)
                    next_point = p;
            }
        }

        boost::mpi::gather(world, next_point, selected_points.data(), 0);

        if (rank == 0) {
            for (auto p : selected_points) {
                if (p == current)
                    continue;
                int val = crossProduct(current, next_point, p);
                if (val > 0) {
                    next_point = p;
                } else if (val == 0) {
                    if (distance(current, next_point, p) < 0)
                        next_point = p;
                }
            }
            result.emplace_back(next_point);
        }
        boost::mpi::broadcast(world, next_point, 0);
        current = next_point;

        if (next_point == start_point)
            break;
    }
    return result;
}

std::vector<P> Jarvis(std::vector<std::pair<int, int>> points) {
    std::pair<int, int> start_point = points[0];
    for (auto p : points)
        if (p.second < start_point.second || (p.second == start_point.second && p.first < start_point.first))
            start_point = p;

    std::vector<P> result;
    std::pair<int, int> current = start_point;
    std::pair<int, int> next_point = points[0];
    while (true) {
        for (auto p : points) {
            if (p == current)
                continue;
            int val = crossProduct(current, next_point, p);
            if (val > 0) {
                next_point = p;
            } else if (val == 0) {
                if (distance(current, next_point, p) < 0)
                    next_point = p;
            }
        }

        result.emplace_back(next_point);
        current = next_point;

        if (next_point == start_point)
            break;
    }
    return result;
}

bool inside_conv(const std::vector<P> &pol, std::vector<std::pair<int, int>> points) {
    int pol_size = pol.size();
    for (auto point_pair : points) {
        P point(point_pair);
        int j = pol_size - 1;
        bool res = false;
        for (int i = 0; i < pol_size; i++) {
            if (pol[i] == point || pol[j] == point || (pol[i].y == pol[j].y && pol[i].y == point.y) ||
                (pol[i].x == pol[j].x && pol[i].x == point.x)) {
                res = true;
                break;
            }
            if ((pol[i].y < point.y && pol[j].y >= point.y || pol[j].y < point.y && pol[i].y >= point.y) &&
                (pol[i].x + (point.y - pol[i].y) / (pol[j].y - pol[i].y) * (pol[j].x - pol[i].x) == point.x)) {
                res = true;
                break;
            }
            if ((pol[i].y < point.y && pol[j].y >= point.y || pol[j].y < point.y && pol[i].y >= point.y) &&
                (pol[i].x + (point.y - pol[i].y) * (pol[j].x - pol[i].x) / (pol[j].y - pol[i].y) > point.x))
                res = !res;
            j = i;
        }
        if (!res)
            return false;
    }
    return true;
}

std::vector<std::vector<int>> create_image(int n, int m) {
    std::random_device rd;
    std::uniform_int_distribution<int> unif(0, 255);
    std::vector<std::vector<int>> image(n, std::vector<int> (m));
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            image[i][j] = unif(rd);
    image[0][0] = 255;  // minimum one point

    return image;
}
