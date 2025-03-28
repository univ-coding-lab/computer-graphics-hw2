#pragma once
#include "Ray.h"

class Surface {
public:
    virtual ~Surface() = default;

    virtual bool intersect(const Ray& ray) const = 0;
};
#pragma once
