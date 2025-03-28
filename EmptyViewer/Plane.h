#pragma once
#include "Surface.h"
#include "Vec3.h"

class Plane : public Surface {
public:
    float y;

    Plane(float height) : y(height) {}

    virtual bool intersect(const Ray& ray) const override {
        if (ray.direction.y == 0) return false;
        float t = (y - ray.origin.y) / ray.direction.y;
        return t > 0;
    }
};
#pragma once
