// Copyright 2023 Kudinov Nikita
#ifndef TASKS_TASK_3_KUDINOV_N_IMAGE_FILTER_GAUSS_BLOCK_IMAGE_FILTER_GAUSS_BLOCK_H_
#define TASKS_TASK_3_KUDINOV_N_IMAGE_FILTER_GAUSS_BLOCK_IMAGE_FILTER_GAUSS_BLOCK_H_

#include <cstdint>
#include <vector>
#include <utility>

using Pixel = std::uint8_t;
using Image = std::vector<std::vector<Pixel>>;

Image generate_random_image(const std::size_t height, const std::size_t width);

std::pair<std::size_t, std::size_t> get_image_segments_dimensions(const std::size_t world_size);

void image_print(const Image& image);

Image image_get_subimage(
    const Image& image,
    const std::size_t y,
    const std::size_t x,
    const std::size_t height,
    const std::size_t width);

Image concat_images_horizontal(const std::vector<Image>& images);
Image concat_images_vertical(const std::vector<Image>& images);

std::vector<std::vector<double>> generate_gauss_kernel(
    const std::size_t kernel_radius,
    const double sigma);

Pixel image_get_gauss_filtered_pixel(
    const Image& image,
    const std::vector<std::vector<double>>& kernel,
    const std::size_t y,
    const std::size_t x);

void image_apply_gauss_filter_on_borders(
    const Image& image,
    const std::vector<std::vector<double>>& kernel,
    Image* out,
    const std::size_t subimage_y,
    const std::size_t subimage_x,
    const std::size_t subimage_height,
    const std::size_t subimage_width);

Image image_gauss_filter_sequential(
    const Image& image,
    const std::size_t kernel_radius = 1,
    const double sigma = 1.0,
    const bool with_borders = true);

Image image_gauss_filter_parallel(
    const Image& image,
    const std::size_t kernel_radius = 1,
    const double sigma = 1.0);

#endif  // TASKS_TASK_3_KUDINOV_N_IMAGE_FILTER_GAUSS_BLOCK_IMAGE_FILTER_GAUSS_BLOCK_H_
