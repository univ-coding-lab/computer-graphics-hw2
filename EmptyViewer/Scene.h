#pragma once
#include <vector>
#include "Surface.h"
#include "Ray.h"

class Scene {
private:
    std::vector<Surface*> objects;

public:
    Scene() {}

    void addObject(Surface* obj) {
        objects.push_back(obj);
    }

    bool closestIntersection(const Ray& ray) const {
        for (const auto& obj : objects) {
            if (obj->intersect(ray)) {
                return true;
            }
        }
        return false;
    }
};
#pragma once
