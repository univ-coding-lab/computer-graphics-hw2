#pragma once
#include "Surface.h"
#include "Vec3.h"

class Sphere : public Surface {
public:
    Vec3 center;
    float radius;

    Sphere(const Vec3& c, float r) : center(c), radius(r) {}

    virtual bool intersect(const Ray& ray) const override {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        return (discriminant > 0);
    }
};
#pragma once
