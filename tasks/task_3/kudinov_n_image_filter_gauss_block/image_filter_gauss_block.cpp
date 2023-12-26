// Copyright 2023 Kudinov Nikita
#include "task_3/kudinov_n_image_filter_gauss_block/image_filter_gauss_block.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <random>
#include <functional>

#include <boost/mpi.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

Image generate_random_image(const std::size_t height, const std::size_t width) {
    std::random_device dev;
    std::mt19937 gen(dev());
    Image out(height, std::vector<Pixel>(width, 0));

    for (std::size_t i = 0; i < height; i += 1) {
        for (std::size_t j = 0; j < width; j += 1) {
            out[i][j] = gen() % 255;
        }
    }

    return out;
}

std::pair<std::size_t, std::size_t> get_image_segments_dimensions(const std::size_t world_size) {
    std::size_t start = std::sqrt(world_size);

    while (world_size % start != 0) {
        start -= 1;
    }

    return std::make_pair(start, world_size / start);
}

void image_print(const Image& image) {
    for (std::size_t i = 0; i < image.size(); i += 1) {
        for (std::size_t j = 0; j < image[i].size(); j += 1) {
            std::cout << std::setw(3) << +image[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

Image image_get_subimage(
    const Image& image,
    const std::size_t y,
    const std::size_t x,
    const std::size_t height,
    const std::size_t width) {
    if (y + height > image.size() || x + width > image[0].size()) {
        throw std::runtime_error("Subimage is out of bounds");
    }

    Image out(height, std::vector<Pixel>(width, 0));

    for (std::size_t i = y; i < y + height; i += 1) {
        for (std::size_t j = x; j < x + width; j += 1) {
            out[i - y][j - x] = image[i][j];
        }
    }

    return out;
}

Image concat_images_horizontal(const std::vector<Image>& images) {
    // Images must be with the same height
    std::size_t out_width = 0;
    for (const Image& image : images) {
        out_width += image[0].size();
    }

    Image out(images[0].size(), std::vector<Pixel>(out_width, 0));

    std::size_t x = 0;
    for (std::size_t k = 0; k < images.size(); k += 1) {
        const Image& image = images[k];

        for (std::size_t i = 0; i < image.size(); i += 1) {
            for (std::size_t j = 0; j < image[i].size(); j += 1) {
                out[i][x + j] = image[i][j];
            }
        }

        x += image[0].size();
    }

    return out;
}

Image concat_images_vertical(const std::vector<Image>& images) {
    // Images must be with the same width
    std::size_t out_height = 0;
    for (const Image& image : images) {
        out_height += image.size();
    }

    Image out(out_height, std::vector<Pixel>(images[0][0].size(), 0));

    std::size_t y = 0;
    for (std::size_t k = 0; k < images.size(); k += 1) {
        const Image& image = images[k];

        for (std::size_t i = 0; i < image.size(); i += 1) {
            for (std::size_t j = 0; j < image[i].size(); j += 1) {
                out[y + i][j] = image[i][j];
            }
        }

        y += image.size();
    }

    return out;
}

std::vector<std::vector<double>> generate_gauss_kernel(
    const std::size_t kernel_radius,
    const double sigma) {
    const std::size_t kernel_size = kernel_radius * 2 + 1;
    std::vector<std::vector<double>> kernel(kernel_size, std::vector<double>(kernel_size, 0.0));

    double sum = 0.0;
    for (std::size_t i = 0; i < kernel_size; i += 1) {
        for (std::size_t j = 0; j < kernel_size; j += 1) {
            const double dist_x = std::pow(static_cast<double>(i - kernel_radius), 2.0);
            const double dist_y = std::pow(static_cast<double>(j - kernel_radius), 2.0);

            const double coef = std::exp(-(dist_x + dist_y) / (2.0 * std::pow(sigma, 2.0)));
            kernel[i][j] = coef;
            sum += coef;
        }
    }

    for (std::size_t i = 0; i < kernel_size; i += 1) {
        for (std::size_t j = 0; j < kernel_size; j += 1) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}

Pixel image_get_gauss_filtered_pixel(
    const Image& image,
    const std::vector<std::vector<double>>& kernel,
    const std::size_t y,
    const std::size_t x) {
    Pixel result_pixel = 0;

    std::size_t kernel_radius = kernel.size() / 2;

    for (std::size_t i = 0; i < kernel.size(); i += 1) {
        for (std::size_t j = 0; j < kernel.size(); j += 1) {
            const Pixel pixel = image
                [std::clamp<std::size_t>(y - kernel_radius + i, 0, image.size() - 1)]
                [std::clamp<std::size_t>(x - kernel_radius + j, 0, image[0].size() - 1)];

            result_pixel += std::clamp<Pixel>(
                kernel[i][j] * static_cast<double>(pixel),
                0,
                UINT8_MAX);
        }
    }

    return result_pixel;
}

void image_apply_gauss_filter_on_borders(
    const Image& image,
    const std::vector<std::vector<double>>& kernel,
    Image* out,
    const std::size_t subimage_y,
    const std::size_t subimage_x,
    const std::size_t subimage_height,
    const std::size_t subimage_width) {
    std::size_t kernel_radius = kernel.size() / 2;

    // Top Left Corner
    for (std::size_t y = subimage_y; y < subimage_y + kernel_radius; y += 1) {
        for (std::size_t x = subimage_x; x < subimage_x + kernel_radius; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Bottom Left Corner
    for (std::size_t y = subimage_y + subimage_height - kernel_radius; y < subimage_y + subimage_height; y += 1) {
        for (std::size_t x = subimage_x; x < subimage_x + kernel_radius; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Top Right Corner
    for (std::size_t y = subimage_y; y < subimage_y + kernel_radius; y += 1) {
        for (std::size_t x = subimage_x + subimage_width - kernel_radius; x < subimage_x + subimage_width; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Bottom Right Corner
    for (std::size_t y = subimage_y + subimage_height - kernel_radius; y < subimage_y + subimage_height; y += 1) {
        for (std::size_t x = subimage_x + subimage_width - kernel_radius; x < subimage_x + subimage_width; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Top Horizontal Border
    for (std::size_t y = subimage_y; y < subimage_y + kernel_radius; y += 1) {
        for (std::size_t x = subimage_x + kernel_radius; x < subimage_x + subimage_width - kernel_radius; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Bottom Horizontal Border
    for (std::size_t y = subimage_y + subimage_height - kernel_radius; y < subimage_y + subimage_height; y += 1) {
        for (std::size_t x = subimage_x + kernel_radius; x < subimage_x + subimage_width - kernel_radius; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Left Vertical Border
    for (std::size_t y = subimage_y + kernel_radius; y < subimage_y + subimage_height - kernel_radius; y += 1) {
        for (std::size_t x = subimage_x; x < subimage_x + kernel_radius; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    // Right Vertical Border
    for (std::size_t y = subimage_y + kernel_radius; y < subimage_y + subimage_height - kernel_radius; y += 1) {
        for (std::size_t x = subimage_x + subimage_width - kernel_radius; x < subimage_x + subimage_width; x += 1) {
            (*out)[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }
}

Image image_gauss_filter_sequential(
    const Image& image,
    const std::size_t kernel_radius,
    const double sigma,
    const bool with_borders) {
    if (image.empty() || image[0].empty()) return {};

    Image out(image.size(), std::vector<Pixel>(image[0].size(), 0));

    const std::vector<std::vector<double>> kernel = generate_gauss_kernel(kernel_radius, sigma);

    for (
        std::size_t y = kernel_radius;
        y < image.size() - kernel_radius;
        y += 1) {
        for (
            std::size_t x = kernel_radius;
            x < image[y].size() - kernel_radius;
            x += 1) {
            out[y][x] = image_get_gauss_filtered_pixel(image, kernel, y, x);
        }
    }

    if (with_borders) {
        image_apply_gauss_filter_on_borders(
            image,
            kernel,
            &out,
            0,
            0,
            image.size(),
            image[0].size());
    }

    return out;
}

Image image_gauss_filter_parallel(
    const Image& image,
    const std::size_t kernel_radius,
    const double sigma) {
    if (image.empty() || image[0].empty()) return {};

    boost::mpi::communicator world;
    std::vector<std::vector<double>> kernel = generate_gauss_kernel(kernel_radius, sigma);

    std::pair<std::size_t, std::size_t> image_segments_dimensions = get_image_segments_dimensions(world.size());
    std::size_t height_segment_size, width_segment_size;
    if (image.size() > image[0].size()) {
        height_segment_size = image_segments_dimensions.second;
        width_segment_size = image_segments_dimensions.first;
    } else {
        height_segment_size = image_segments_dimensions.first;
        width_segment_size = image_segments_dimensions.second;
    }

    std::size_t segment_height = image.size() / height_segment_size;
    std::size_t segment_width = image[0].size() / width_segment_size;

    std::vector<Image> segments;
    Image local_segment;
    if (world.rank() == 0) {
        std::size_t i, segment_position_y;
        for (i = 0, segment_position_y = 0; i < height_segment_size; i += 1, segment_position_y += segment_height) {
            std::size_t new_segment_height = i == height_segment_size - 1
                ? image.size() - segment_position_y
                : segment_height;

            std::size_t j, segment_position_x;
            for (j = 0, segment_position_x = 0; j < width_segment_size; j += 1, segment_position_x += segment_width) {
                std::size_t new_segment_width = j == width_segment_size - 1
                    ? image[0].size() - segment_position_x
                    : segment_width;

                Image subimage = image_get_subimage(
                    image,
                    segment_position_y,
                    segment_position_x,
                    new_segment_height,
                    new_segment_width);

                segments.push_back(subimage);
            }
        }
    }

    boost::mpi::scatter(world, segments, local_segment, 0);

    Image local_result = image_gauss_filter_sequential(local_segment, kernel_radius, sigma, false);

    std::vector<Image> global_result;
    boost::mpi::gather(world, local_result, global_result, 0);

    Image gathered_image;
    if (world.rank() == 0) {
        std::vector<Image> rows;
        std::size_t count = 0;
        for (std::size_t i = 0; i < height_segment_size; i += 1) {
            std::vector<Image> horizontal_segments;

            for (std::size_t j = 0; j < width_segment_size; j += 1) {
                horizontal_segments.push_back(global_result[count]);
                count += 1;
            }

            rows.push_back(concat_images_horizontal(horizontal_segments));
        }

        gathered_image = concat_images_vertical(rows);

        std::size_t i, segment_position_y;
        for (i = 0, segment_position_y = 0; i < height_segment_size; i += 1, segment_position_y += segment_height) {
            std::size_t new_segment_height = i == height_segment_size - 1
                ? image.size() - segment_position_y
                : segment_height;

            std::size_t j, segment_position_x;
            for (j = 0, segment_position_x = 0; j < width_segment_size; j += 1, segment_position_x += segment_width) {
                std::size_t new_segment_width = j == width_segment_size - 1
                    ? image[0].size() - segment_position_x
                    : segment_width;

                image_apply_gauss_filter_on_borders(
                    image,
                    kernel,
                    &gathered_image,
                    segment_position_y,
                    segment_position_x,
                    new_segment_height,
                    new_segment_width);
            }
        }
    }

    return gathered_image;
}
