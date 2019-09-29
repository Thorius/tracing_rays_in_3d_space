#pragma once

#include <cmath>

#include "constants.hpp"
#include "ray.hpp"

class Camera {
    public:
        /// Construct a camera at the given position, looking at a specific point, with a given up vector,
        /// a specified field of view and aspect ratio.
        Camera(Vector3 const& lookfrom, Vector3 const& lookat, Vector3 const& vup, float vfov, float aspect) {
            float theta = float(vfov * pi/180);
            float half_height = std::tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            Vector3 w = unit_vector(lookfrom - lookat);
            Vector3 u = unit_vector(cross(vup, w));
            Vector3 v = cross(w, u);
            lower_left_corner = origin  - half_width*u - half_height*v - w;
            horizontal = 2*half_width*u;
            vertical = 2*half_height*v;
        }

        /// Given two values in the range [0, 1] denoting coordinates on the plane in front of the camera,
        /// generate a ray from the center of the camera, passing through those coordinates. 
        Ray get_ray(float s, float t) {
            return Ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

        Vector3 origin;
        Vector3 lower_left_corner;
        Vector3 horizontal;
        Vector3 vertical;

};
