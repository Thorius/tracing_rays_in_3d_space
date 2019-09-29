#pragma once

#include "material.hpp"
#include "ray.hpp"
#include "vector3.hpp"

class Metal : public Material {
  public:
    Metal(Vector3 const &attenuation) : albedo(attenuation) {}
    bool scatter(Ray const &ray, HitRecord const &record, Vector3 &attenuation,
                 Ray &scattered) const override;

  private:
    Vector3 albedo;
};

bool Metal::scatter(Ray const &ray, HitRecord const &record,
                    Vector3 &attenuation, Ray &scattered) const {
    Vector3 reflected = reflect(unit_vector(ray.direction()), record.normal);
    scattered = Ray(record.p, reflected);
    attenuation = albedo;
    // There is a scattered ray only if the external ray has hit the surface on
    // the outside
    return dot(scattered.direction(), record.normal) > 0.0f;
}
