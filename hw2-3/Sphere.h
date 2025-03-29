#pragma once
#include "Surface.h"
#include "Vec3.h"

class Sphere : public Surface {
public:
    Vec3 center;
    float radius;

    Sphere(const Vec3& c, float r) : center(c), radius(r) {}

    virtual bool intersect(const Ray& ray, float& t, Vec3& normal) const override {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            float sqrtDisc = std::sqrt(discriminant);
            float t0 = (-b - sqrtDisc) / (2.0f * a);
            if (t0 > 0.001f) {
                t = t0;
                Vec3 p = ray.at(t);
                normal = (p - center).normalized();
                return true;
            }
        }
        return false;
    }
};
#pragma once
