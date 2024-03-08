#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    point3 o;
    vec3 d;

public:
    ray() = default;
    ray(const point3 &origin, const vec3 &direction) : o{origin}, d{direction} {}

    point3 origin() const { return o; }
    vec3 direction() const { return d; }

    point3 at(double t) const {
        return o + t * d;
    }
};

#endif
