// Copyright 2023 Safarov Nurlan
#include "task_2/safarov_n_image_smoothing/image_smoothing.h"

Image::Image(int _width, int _height) {
    pixels.reserve(_width*_height);
    height = _height;
    width = _width;
    for (int i = 0; i < height * width; i++) {
        pixels.push_back(0);
    }
}

int Image::GetWidth() const { return width; }

int Image::GetHeight() const { return height; }

void Image::FillRandom() {
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < width*height; i++) {
        pixels[i] = (unsigned char)(gen() % 256);
    }
}

unsigned char Image::GetPixel(int x, int y) const {
    if (x < 0) {
        x = 0;
    } else if (x >= width) {
        x = width - 1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= height) {
        y = height - 1;
    }

    return pixels[y*width + x];
}

void Image::SetPixel(int x, int y, unsigned char value) {
    pixels[y*width + x] = value;
}

void Image::Show() {
    for (int i = 0; i < GetWidth() * GetHeight(); i++) {
        if (i % GetWidth() == 0) {
            std::cout << "\n";
        }
        std::cout << static_cast<int>(pixels[i]) << "\t";
    }
}

bool Image::operator==(const Image& other) const {
    if (pixels == other.pixels) {
        return true;
    }
    return false;
}

std::vector<Point> GetPoints() {
    std::vector<Point> Points;
    Points.push_back(Point(-1, 1));
    Points.push_back(Point(0, 1));
    Points.push_back(Point(1, 1));

    Points.push_back(Point(-1, 0));
    Points.push_back(Point(0, 0));
    Points.push_back(Point(1, 0));

    Points.push_back(Point(-1, -1));
    Points.push_back(Point(0, -1));
    Points.push_back(Point(1, -1));

    return Points;
}

Image GetCustomImage() {
    Image image(3, 3);
    image.SetPixel(0, 0, 255);
    image.SetPixel(1, 0, 0);
    image.SetPixel(2, 0, 0);
    image.SetPixel(0, 1, 0);
    image.SetPixel(1, 1, 255);
    image.SetPixel(2, 1, 0);
    image.SetPixel(0, 2, 0);
    image.SetPixel(1, 2, 0);
    image.SetPixel(2, 2, 0);

    return image;
}

Image SmoothingSequential(const Image& source) {
    Image result(source.GetWidth(), source.GetHeight());
    auto Points = GetPoints();

    for (int x = 0; x < source.GetWidth(); x++) {
        for (int y = 0; y < source.GetHeight(); y++) {
            int averagePixel = 0;
            for (int i = 0; i < Points.size(); i++) {
                int pixelX = x + Points[i].x;
                int pixelY = y + Points[i].y;
                averagePixel += source.GetPixel(pixelX, pixelY);
            }
            averagePixel /= 9;
            result.SetPixel(x, y, (unsigned char)averagePixel);
        }
    }

    return result;
}

Image SmoothingParallel(const Image& source) {
    int rank, pCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &pCount);

    std::vector<Point> Points = GetPoints();

    if (rank == 0) {
        Image result(source.GetWidth(), source.GetHeight());
        int image_size = source.GetHeight() * source.GetWidth();
        int pixels_for_process = image_size / pCount;
        int pixels_for_main_process =
            pixels_for_process + (image_size % pCount);
        int current_pixel = 0;
        unsigned char* main_process_result =
            new unsigned char[pixels_for_main_process];

        for (int i = 1; i < pCount; i++) {
            unsigned char* arr = new unsigned char[pixels_for_process*9];
            for (int loc_pix_number = 0; loc_pix_number < pixels_for_process;
                loc_pix_number++, current_pixel++) {
                int x = current_pixel % source.GetWidth();
                int y = current_pixel / source.GetWidth();

                for (int point_number = 0; point_number < Points.size();
                    point_number++) {
                    int pixel_x = x + Points[point_number].x;
                    int pixel_y = y + Points[point_number].y;
                    arr[loc_pix_number * 9 + point_number] =
                        source.GetPixel(pixel_x, pixel_y);
                }
            }

            int size = pixels_for_process * 9;
            MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(arr, pixels_for_process * 9, MPI_UNSIGNED_CHAR, i, 0,
                MPI_COMM_WORLD);
        }

        for (int loc_pix_number = 0; current_pixel < image_size;
            loc_pix_number++, current_pixel++) {
            int x = current_pixel % source.GetWidth();
            int y = current_pixel / source.GetWidth();
            int average = 0;

            for (int point_number = 0; point_number < Points.size();
                point_number++) {
                int pixel_x = x + Points[point_number].x;
                int pixel_y = y + Points[point_number].y;
                average += source.GetPixel(pixel_x, pixel_y);
            }
            main_process_result[loc_pix_number] = (unsigned char)(average / 9);
        }

        current_pixel = 0;
        for (int i = 1; i < pCount; i++) {
            unsigned char* accepted_pixels =
                new unsigned char[pixels_for_process];
            MPI_Recv(accepted_pixels, pixels_for_process, MPI_UNSIGNED_CHAR, i,
                0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            for (int loc_pix_number = 0; loc_pix_number < pixels_for_process;
                loc_pix_number++, current_pixel++) {
                int x = current_pixel % source.GetWidth();
                int y = current_pixel / source.GetWidth();
                result.SetPixel(x, y, accepted_pixels[loc_pix_number]);
            }
        }
        for (int loc_pix_number = 0; current_pixel < image_size;
            loc_pix_number++, current_pixel++) {
            int x = current_pixel % source.GetWidth();
            int y = current_pixel / source.GetWidth();
            result.SetPixel(x, y, main_process_result[loc_pix_number]);
        }

        return result;
    } else {
        int size;
        unsigned char* arr;
        unsigned char* result;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        arr = new unsigned char[size];
        result = new unsigned char[size / 9];
        MPI_Recv(arr, size, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD,
            MPI_STATUSES_IGNORE);

        for (int i = 0; i < size / 9; i++) {
            int average = 0;
            for (int j = 0; j < 9; j++) {
                average += arr[i * 9 + j];
            }
            result[i] = (unsigned char)(average / 9);
        }

        MPI_Send(result, size / 9, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);

        return Image(1, 1);
    }
}
