#pragma once

#include "hittable.hpp"

#include <cmath>

class Sphere: public Hittable  {
    public:
        Sphere(Vector3 cen, float r, Material const& mat) : center(cen), radius(r), material(&mat) {};
        virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
        Vector3 center;
        float radius;
        Material const* material;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Vector3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - std::sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            return true;
        }
        temp = (-b + std::sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            return true;
        }
    }
    return false;
}
