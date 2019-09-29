#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "vector3.hpp"
#include "ray.hpp"
#include "camera.hpp"

#include "TinyPngOut.hpp" // For writing png files.

using Color = Vector3;
using Film = std::vector<std::vector<Color>>;

// Resise the given film to width times height image.
void resize_film(Film &film, unsigned const width, unsigned height) {
    film.resize(height);
    for (auto &row : film) {
        row.resize(width);
    }
}

bool output_ppm_image(std::string const &filename, Film const &image) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Unable to open file\n";
        return false;
    }
    out << "P3\n" << image.front().size() << ' ' << image.size() << "\n255\n";
    for (auto curr = image.rbegin(); curr != image.rend(); ++curr) {
        for (auto pixel : *curr) {
            pixel *= 255.0f; // Transform from [0, 1] range to [0, 255]
            out << static_cast<int>(pixel.r()) << ' '
                << static_cast<int>(pixel.g()) << ' '
                << static_cast<int>(pixel.b()) << '\n';
        }
    }
    return true;
}

bool output_png_image(std::string const &filename, Film const &image) {
    std::uint32_t imageWidth = std::uint32_t(image[0].size());
    std::uint32_t imageHeight = std::uint32_t(image.size());
    // Convert the Film object to an array of RGB (red, green, blue) pixels with
    // values between 0 and 255.
    auto numPixels = 3 * imageWidth * imageHeight;
    std::vector<std::uint8_t> rawPixels(numPixels);
    std::size_t currentPixel = 0;
    for (auto curr = image.rbegin(); curr != image.rend(); ++curr) {
        for (auto pixel : *curr) {
            pixel *= 255.0f; // Transform from [0, 1] range to [0, 255]
            rawPixels[currentPixel++] = std::clamp(int(pixel.r()), 0, 255);
            rawPixels[currentPixel++] = std::clamp(int(pixel.g()), 0, 255);
            rawPixels[currentPixel++] = std::clamp(int(pixel.b()), 0, 255);
        }
    }
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Unable to open file\n";
        return false;
    }
    TinyPngOut pngout(imageWidth, imageHeight, out);
    pngout.write(rawPixels.data(), imageWidth * imageHeight);
    return true;
}

// Color function
Vector3 scene_color(Ray const& r) {
    Vector3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.45f, 0.65f, 1.0f);
}

int main()
{
    // Image parameters
    unsigned const nx = 200;
    unsigned const ny = 100;
    Film output;
    // Camera parameters
    Vector3 origin(0, 0, 0);
    Vector3 lookat(4, 0, 0);
    Vector3 up(0, 1, 0);
    Camera cam(origin, lookat, up, 90, float(nx) / float(ny));

    resize_film(output, nx, ny);
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            auto u = float(i) / float(nx);
            auto v = float(j) / float(ny);
            auto col = scene_color(cam.get_ray(u, v));
            output[j][i] = col;
        }
    }
    // Write image files
    if (!output_ppm_image("camera.ppm", output)) {
        return 1;
    }
    if (!output_png_image("camera.png", output)) {
        return 1;
    }
    return 0;
}
