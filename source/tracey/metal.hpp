#pragma once 

#include "material.hpp"
#include "vector3.hpp"
#include "ray.hpp"

class Metal : public Material {
public:
    Metal(Vector3 const& attenuation) : albedo(attenuation) {}
    bool scatter(Ray const& ray, HitRecord const& record, Vector3 & attenuation, Ray & scattered) const override;

private:
    Vector3 albedo;
};

bool Metal::scatter(Ray const& ray, HitRecord const& record, Vector3 & attenuation, Ray & scattered) const
{
    // TODO: Write scatter function
    return false;
}
