#pragma once 

#include "hittable.hpp"

#include <vector>

class Composite : public Hittable  {
    public:
        Composite() = default;
        virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
        void add_hittable(Hittable & h) {
            hittables.push_back(&h);
        }
        std::vector<Hittable *> hittables;
};

bool Composite::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = tmax;
    for (auto & curr : hittables) {
        if (curr->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
