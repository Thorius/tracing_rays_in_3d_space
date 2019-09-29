#pragma once

#include <random>

#include "vector3.hpp"

inline double random_number()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

inline Vector3 random_vector() {
    return Vector3(float(random_number()), float(random_number()), float(random_number()));
}

inline Vector3 random_vector_in_unit_sphere() {
    Vector3 p;
    do {
        p = 2.0f * random_vector() - Vector3(1,1,1);
    } while (dot(p,p) >= 1.0f);
    return p;
}
