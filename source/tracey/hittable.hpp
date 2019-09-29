#pragma once

#include "Ray.hpp"

class Material;

struct HitRecord {
    float t;
    Vector3 p;
    Vector3 normal;
    Material const* material;
};

class Hittable  {
public:
    virtual bool hit(Ray const& r, float t_min, float t_max, HitRecord & rec) const = 0;
    virtual ~Hittable() = default;
};
