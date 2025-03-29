#pragma once
#include "Vec3.h"

class Ray {
public:
    Vec3 origin;
    Vec3 direction;

    Ray() {}
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}

    Vec3 at(float t) const {
        return direction * t + origin;
    }
};
#pragma once
