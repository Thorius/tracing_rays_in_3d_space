#pragma once

#include "vector3.hpp"

class Ray
{
    public:
        Ray() = default;
        Ray(Vector3 const& a, Vector3 const& b) : orig(a), dir(b) {}
        Vector3 const& origin() const       { return orig; }
        Vector3 const& direction() const    { return dir; }
        Vector3 point_at_parameter(float t) const { return orig + t*dir; }
    private:
        Vector3 orig;
        Vector3 dir;
};

/// Reflect the vector v around the vector n (assuming the tip of v is at the base of v).
inline Vector3 reflect(Vector3 const& v, Vector3 const& n)
{
    return v - 2.0f * dot(v, n) * n;
}
