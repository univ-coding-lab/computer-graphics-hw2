#pragma once
#include "Vec3.h"
#include "Ray.h"

class Camera {
public:
    Vec3 eye;
    Vec3 u, v, w;
    float l, r, b, t, d;
    int nx, ny;

    Camera(
        const Vec3& eye_,
        const Vec3& u_, const Vec3& v_, const Vec3& w_,
        float l_, float r_, float b_, float t_, float d_,
        int nx_, int ny_)
        : eye(eye_), u(u_), v(v_), w(w_),
        l(l_), r(r_), b(b_), t(t_), d(d_),
        nx(nx_), ny(ny_) {
    }

    Ray generateRay(int i, int j) const {
        float u_scalar = l + (r - l) * (i + 0.5f) / nx;
        float v_scalar = b + (t - b) * (j + 0.5f) / ny;

        Vec3 direction = (u * u_scalar) + (v * v_scalar) - (w * d);
        return Ray(eye, direction.normalized());
    }
};
#pragma once
