#pragma once
#include "Ray.h"

struct Light {
    Vec3 position;
    Vec3 intensity;
};

struct Material {
    Vec3 ka, kd, ks;
    float specular_power;
};

class Surface {
public:
    Material material;

    virtual ~Surface() = default;

    virtual bool intersect(const Ray& ray, float& t, Vec3& normal) const = 0;
};
#pragma once
