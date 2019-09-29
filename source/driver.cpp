#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "camera.hpp"
#include "composite.hpp"
#include "hittable.hpp"
#include "lambertian.hpp"
#include "material.hpp"
#include "metal.hpp"
#include "sampling.hpp"
#include "sphere.hpp"
#include "vector3.hpp"

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

/// Maximum reflection depth.
constexpr int const max_depth = 30;

// Color function, which given a ray and an "world" returns the color which
// would be seen by the given ray.
Vector3 scene_color(Ray const &r, Hittable &world, int depth) {
    HitRecord record;
    if (world.hit(r, 0.001f, std::numeric_limits<float>::max(), record)) {
        Ray scattered;
        Vector3 attenuation;
        Vector3 target =
            record.p + record.normal + random_vector_in_unit_sphere();
        if (depth < max_depth &&
            record.material->scatter(r, record, attenuation, scattered)) {
            return attenuation * scene_color(scattered, world, depth + 1);
        } else {
            return Vector3(0.0f, 0.0f, 0.0f);
        }
    } else {
        // Sky color
        Vector3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * Vector3(1.0f, 1.0f, 1.0f) +
               t * Vector3(0.45f, 0.65f, 1.0f);
    }
}

// Gamma correction - a lot of image viewers assume that the output is gamma
// corrected. For our purposes, just applying sqrt to each of the values is
// sufficient.
Vector3 gamma_correction(Vector3 const &color) {
    return Vector3(std::sqrt(color.r()), std::sqrt(color.g()),
                   std::sqrt(color.b()));
}

int main(int argc, const char *argv[]) {
    // Image parameters
    int const nx = 900;
    int const ny = 600;
    int const ns = 50;
    Film output;
    resize_film(output, nx, ny);
    // Camera parameters
    Vector3 origin(0, 0.5, 1);
    Vector3 lookat(0, 0, -4);
    Vector3 up(0, 1, 0);
    Camera cam(origin, lookat, up, 100, float(nx) / float(ny));
    // Actual scene
    Composite world;
    Lambertian sphere_mat_1(Vector3(0.8f, 0.2f, 0.2f));
    Sphere sphere_small_1(Vector3(0.0f, 0.0f, -1.0f), 0.5f, sphere_mat_1);
    Metal sphere_mat_2(Vector3(0.7f, 0.8f, 0.2f));
    Sphere sphere_small_2(Vector3(-1.1f, 0.0f, -1.0f), 0.5f, sphere_mat_2);
    Metal sphere_mat_3(Vector3(0.7f, 0.7f, 0.9f));
    Sphere sphere_small_3(Vector3(1.1f, 0.0f, -1.0f), 0.5f, sphere_mat_3);
    Lambertian sphere_mat_4(Vector3(0.9f, 0.1f, 0.2f));
    Sphere sphere_small_4(Vector3(3.1f, 2.0f, 4.0f), 0.5f, sphere_mat_4);
    Lambertian sphere_large_mat_1(Vector3(0.8f, 0.9f, 0.1f));
    Sphere sphere_large_1(Vector3(0.0f, -50.5f, 1.0f), 50.0f,
                          sphere_large_mat_1);
    Metal sphere_large_mat_2(Vector3(0.3f, 0.3f, 0.9f));
    Sphere sphere_large_2(Vector3(5.0f, 26.5f, -7.0f), 25.0f,
                          sphere_large_mat_2);
    world.add_hittable(sphere_small_1);
    world.add_hittable(sphere_small_2);
    world.add_hittable(sphere_small_3);
    world.add_hittable(sphere_small_4);
    world.add_hittable(sphere_large_1);
    world.add_hittable(sphere_large_2);
    // Progress monitoring variables
    int progressBarTick = 10;
    int numProgressBarTicks = ny / progressBarTick;
    std::cout << "Progress:\n|" << std::string(numProgressBarTicks, '=')
              << "|\n|";
    // Render loop
    for (int j = ny - 1; j >= 0; --j) { // Iterate over each row in reverse
        for (int i = 0; i < nx; ++i) {  //  Iterate over each column
            // Color into which we will accumulate
            Color accum(0, 0, 0);
            for (int s = 0; s != ns;
                 ++s) { // Take several random samples for the pixel
                auto u = float(i + random_number()) / float(nx);
                auto v = float(j + random_number()) / float(ny);
                // For the given camera ray,
                accum += scene_color(cam.get_ray(u, v), world, 0);
            }
            accum /= float(ns);
            output[j][i] = gamma_correction(accum);
        }
        // Update progress
        if (j % progressBarTick == 0) {
            std::cout << '=';
        }
    }
    // Tidy up progress monitoring output
    std::cout << "|\n";
    // Write image files
    if (!output_ppm_image("simple_scene_2.ppm", output)) {
        return 1;
    };
    if (!output_png_image("simple_scene_2.png", output)) {
        return 1;
    }
    return 0;
}
