#pragma once

#include "hittable.hpp"

#include <cmath>

class Sphere: public Hittable  {
    public:
        Sphere(Vector3 cen, float r) : center(cen), radius(r) {};
        virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
        Vector3 center;
        float radius;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    // TODO
    return false;
}
