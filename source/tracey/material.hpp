#pragma once 

#include "vector3.hpp"
#include "ray.hpp"
#include "hittable.hpp"

class Material {
public:
    virtual bool scatter(Ray const& ray, HitRecord const& record, Vector3 & attenuation, Ray & scattered) const = 0;
    virtual ~Material() {}
};
