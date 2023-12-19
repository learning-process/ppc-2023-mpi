// Copyright 2023 Volodin Evgeniy
#include "./chbi.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

void floodFill(std::vector<int>* image, int height, int width, int yStart, int xStart, int label) {
    std::queue<point2d> tasks;
    tasks.push(point2d(xStart, yStart));
    while (!tasks.empty()) {
        int x = tasks.front().x;
        int y = tasks.front().y;
        tasks.pop();
        if (x >= 0 && y >= 0 && y < height && x < width && image->at(y * width + x) == 1) {
            (*image)[y * width + x] = label;
            tasks.push(point2d(x - 1, y - 1));
            tasks.push(point2d(x - 1, y));
            tasks.push(point2d(x - 1, y + 1));
            tasks.push(point2d(x, y + 1));
            tasks.push(point2d(x + 1, y + 1));
            tasks.push(point2d(x + 1, y));
            tasks.push(point2d(x, y - 1));
            tasks.push(point2d(x + 1, y - 1));
        }
    }
}

std::vector<int> findComponents(const std::vector<std::vector<int>>& image, int width, int height) {
    std::vector<int> image_with_components(width * height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image_with_components[i * width + j] = image[i][j];
        }
    }
    int label = 2;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (image_with_components[i * width + j] == 1) {
                floodFill(&image_with_components, height, width, i, j, label);
                ++label;
            }
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(image_with_components[i * width + j] != 0) {
                image_with_components[i * width + j]--;
            }
        }
    }
    return image_with_components;
}

int findCountComponents(const std::vector<int> &image) {
    int count_components = 0;
    for (int i = 0; i < image.size(); ++i) {
        if (image[i] > count_components) {
            count_components = image[i];
        }
    }
    return count_components;
}

int findCountPointsInComponent(const std::vector<int> &image) {
    int count_points = 0;
    for (int i = 0; i < image.size(); ++i) {
        if (image[i] != 0) {
            count_points++;
        }
    }
    return count_points;
}

std::vector<int> removeExtraPoints(const std::vector<int> &image, int width, int height, int label) {
    std::vector<int> local_image(image);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (image[i * width + j] == label) {
                if ((j > 0) && (j < width - 1)) {
                    if ((i == 0) || (i == height - 1)) {
                        if ((image[i * width + j - 1] == label) && (image[i * width + j + 1] == label)) {
                            local_image[i * width + j] = 0;
                        }
                    }
                    if ((i > 0) && (i < height - 1)) {
                        if (((image[i * width + j - 1] == label) && (image[i * width + j + 1] == label)) ||
                            ((image[(i + 1) * width + j] == label) && (image[(i - 1) * width + j] == label))) {
                            local_image[i * width + j] = 0;
                        }
                    }
                    continue;
                }
                if ((i > 0) && (i < height - 1)) {
                    if ((j == 0) || (j == width - 1)) {
                        if ((image[(i - 1) * width + j] == label) &&
                            (image[(i + 1) * width + j] == label)) {
                            local_image[i * width + j] = 0;
                        }
                    }
                    if ((j > 0) && (j < width - 1)) {
                        if (((image[i * width + j - 1] == label) && (image[i * width + j + 1] == label)) ||
                            ((image[(i + 1) * width + j] == label) && (image[(i - 1) * width + j] == label))) {
                            local_image[i * width + j] = 0;
                        }
                    }
                }
            } else {
                local_image[i * width + j] = 0;
            }
        }
    }
    int size = findCountPointsInComponent(local_image);
    std::vector<int> points(size * 2);
    int k = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (local_image[i * width + j] != 0) {
                points[k] = j;
                k++;
                points[k] = i;
                k++;
            }
        }
    }
    return points;
}

int cross(int x1, int y1, int x2, int y2, int x3, int y3) {
    return ((x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1));
}

void sort(std::vector<int>* points, int xMin, int yMin) {
    int size = points->size() / 2;
    for (int i = 1; i < size; ++i) {
        int j = i;
        while ((j > 0) && (cross(xMin, yMin, (*points)[2 * j - 2],
            (*points)[2 * j - 1], (*points)[2 * j], (*points)[2 * j + 1]) < 0)) {
            int temp = (*points)[2 * j - 2];
            (*points)[2 * j - 2] = (*points)[2 * j];
            (*points)[2 * j] = temp;
            temp = (*points)[2 * j - 1];
            (*points)[2 * j - 1] = (*points)[2 * j + 1];
            (*points)[2 * j + 1] = temp;
            j--;
        }
    }
}

std::vector<int> graham(std::vector<int> points) {
    std::vector<int> result;
    int num_points = points.size() / 2;
    if (num_points > 1) {
        int x_min = points[0];
        int y_min = points[1];
        int min_index = 0;
        for (int i = 2; i < points.size(); i+=2) {
            if (points[i] < x_min || (points[i] == x_min && points[i + 1] < y_min)) {
                x_min = points[i];
                y_min = points[i + 1];
                min_index = i;
            }
        }
        int temp = points[min_index];
        points[min_index] = points[num_points * 2 - 2];
        points[num_points * 2 - 2] = temp;
        temp = points[min_index + 1];
        points[min_index + 1] = points[num_points * 2 - 1];
        points[num_points * 2 - 1] = temp;
        points.pop_back();
        points.pop_back();
        sort(&points, x_min, y_min);
        result.push_back(x_min);
        result.push_back(y_min);
        result.push_back(points[0]);
        result.push_back(points[1]);
        for (int i = 2; i < points.size(); i += 2) {
            int result_size = result.size();
            int x1 = result[result_size - 4];
            int y1 = result[result_size - 3];
            int x2 = result[result_size - 2];
            int y2 = result[result_size - 1];
            int x3 = points[i];
            int y3 = points[i + 1];

            int rot = cross(x1, y1, x2, y2, x3, y3);
            if (rot == 0) {
                result[result_size - 2] = x3;
                result[result_size - 1] = y3;
            } else if (rot < 0) {
                while (cross(result[(result.size()) - 4], result[(result.size()) - 3],
                            result[(result.size()) - 2], result[(result.size()) - 1], x3, y3) < 0)
                    result.pop_back(), result.pop_back();
                result.push_back(x3);
                result.push_back(y3);
            } else {
                result.push_back(x3);
                result.push_back(y3);
            }
        }
    } else {
        result.resize(2);
        result[0] = points[0];
        result[1] = points[1];
    }
    return result;
}

std::vector<int> getConvexHullSeq(const std::vector<std::vector<int>> &image, int width, int height) {
    std::vector<int> convex_hull;
    std::vector<int> local_image = findComponents(image, width, height);
    int count_components = findCountComponents(local_image);
    for (int i = 1; i <= count_components; ++i) {
        std::vector<int> points = removeExtraPoints(local_image, width, height, i);
        std::vector<int> ch = graham(points);
        for (int j = 0; j < ch.size(); ++j) {
            convex_hull.push_back(ch[j]);
        }
        convex_hull.push_back(-1);
    }
    return convex_hull;
}

std::vector<int> getConvexHullPar(const std::vector<std::vector<int>> &image, int width, int height) {
    std::vector<int> convex_hull;

    boost::mpi::communicator world;
    std::vector<int> local_image(width * height);
    
    int count_components;

    if (world.rank() == 0) {
        local_image = findComponents(image, width, height);
        count_components = findCountComponents(local_image);
        for (int i = 1; i < world.size(); ++i) {
            world.send(i, 0, &count_components, 1);
        }
    } else {
        world.recv(0, 0, &count_components, 1);
    }

    const int proc_num = std::min(world.size(), count_components);
    if (world.rank() >= proc_num) {
        return std::vector<int>(0);
    }

    std::vector<int> components(count_components);
    for (int i = 0; i < count_components; ++i) {
        components[i] = i + 1;
    }

    int chunk = count_components / proc_num;
    int rem = count_components % proc_num;
    if (world.rank() < rem) {
        chunk++;
    }

    std::vector<int> local_components(chunk);

    if (world.rank() == 0) {
        for (int i = 1; i < proc_num; ++i) {
            world.send(i, 0, local_image.data(), width * height);
        }
    } else {
        world.recv(0, 0, local_image.data(), width * height);
    }
    if (world.rank() != 0) {
        world.send(0, 0, &chunk, 1);
    }
    if (world.rank() == 0) {
        int index = 0;
        for (int i = 1; i < proc_num; ++i) {
            int proc_chunk;
            world.recv(i, 0, &proc_chunk, 1);
            world.send(i, 0, components.data() + index, proc_chunk);
            index += proc_chunk;
        }
        for (int i = index, j = 0; i < count_components; ++i, ++j) {
            local_components[j] = components[i];
        }
    } else {
        world.recv(0, 0, local_components.data(), chunk);
    }
    std::vector<int> local_convex_hull;
    for (int i = 0; i < local_components.size(); ++i) {
        std::vector<int> points = removeExtraPoints(local_image, width, height, local_components[i]);
        std::vector<int> ch = graham(points);
        for (int j = 0; j < ch.size(); ++j) {
            local_convex_hull.push_back(ch[j]);
        }
        local_convex_hull.push_back(-1);
    }
    if (world.rank() == 0) {
        int size = 2 * findCountPointsInComponent(local_image);
        size += findCountComponents(local_image);
        for (int i = 1; i < proc_num; ++i) {
            std::vector<int> buffer(size);
            world.recv(i, 0, buffer.data(), size);
            for (int i = 0; i < size; ++i) {
                if (buffer[i] == -1 && buffer[i+1] == 0) {
                    convex_hull.push_back(buffer[i]);
                    break;
                }
                convex_hull.push_back(buffer[i]);
            }
        }
        for (int i = 0; i < local_convex_hull.size(); ++i) {
            convex_hull.push_back(local_convex_hull[i]);
        }
    } else {
        world.send(0, 0, local_convex_hull.data(), local_convex_hull.size());
    }
    return convex_hull;
}

void fillImageRandom(std::vector<std::vector<int>>* image, int width, int height) {
    std::default_random_engine rnd;
    std::uniform_int_distribution<> dist(0, 1);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel = dist(rnd);
        }
    }
}

