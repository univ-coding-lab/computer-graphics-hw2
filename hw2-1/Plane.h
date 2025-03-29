#pragma once
#include "Surface.h"
#include "Vec3.h"

class Plane : public Surface {
public:
    float y;

    Plane(float height) : y(height) {}

    virtual bool intersect(const Ray& ray, float& t, Vec3& normal) const override {
        if (ray.direction.y == 0) return false;
        t = (y - ray.origin.y) / ray.direction.y;
        if (t > 0.001f) {
            normal = Vec3(0, 1, 0);
            return true;
        }
        return false;
    }
};
#pragma once
