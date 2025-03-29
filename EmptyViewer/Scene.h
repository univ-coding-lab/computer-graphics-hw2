#pragma once
#include <vector>
#include "Surface.h"
#include "Ray.h"

struct HitInfo {
    Surface* object;
    float t;
    Vec3 normal;
    Vec3 point;
    bool hit;

    HitInfo() : object(nullptr), t(0), hit(false) {}
};

class Scene {
private:
    std::vector<Surface*> objects;

public:
    Scene() {}

    void addObject(Surface* obj) {
        objects.push_back(obj);
    }

    HitInfo closestIntersection(const Ray& ray) const {
        HitInfo result;
        float closestT = 1e9;

        for (Surface* obj : objects) {
            float t;
            Vec3 normal;
            if (obj->intersect(ray, t, normal) && t < closestT) {
                closestT = t;
                result.hit = true;
                result.object = obj;
                result.t = t;
                result.normal = normal;
                result.point = ray.at(t);
            }
        }
        return result;
    }

    bool isInShadow(const Vec3& point, const Light& light) const {
        Vec3 lightDir = (light.position - point).normalized();
        Ray shadowRay(point + lightDir * 0.001f, lightDir);

        for (Surface* obj : objects) {
            float t;
            Vec3 normal;
            if (obj->intersect(shadowRay, t, normal)) {
                return true;
            }
        }
        return false;
    }

    std::vector<Surface*> getObjects()
    {
        return objects;
    }
};
#pragma once
