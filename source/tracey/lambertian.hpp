#pragma once

#include "material.hpp"
#include "vector3.hpp"
#include "ray.hpp"
#include "sampling.hpp"

class Lambertian : public Material {
public:
    Lambertian(Vector3 const& attenuation) : albedo(attenuation) {}
    bool scatter(Ray const& ray, HitRecord const& record, Vector3 & attenuation, Ray & scattered) const override;

private:
    Vector3 albedo;
};

bool Lambertian::scatter(Ray const& ray, HitRecord const& record, Vector3 & attenuation, Ray & scattered) const
{
    Vector3 target = record.p + record.normal + random_vector_in_unit_sphere();
    scattered = Ray(record.p, target - record.p);
    attenuation = albedo;
    return true;
}
