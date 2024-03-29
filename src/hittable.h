#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray-tracing-in-one-weekend.h"

class material;

class hit_record {
public:
    point3 p;
    vec3 n;
    shared_ptr<material> mat;
    double t;
    bool front_face;

    // outward normal is assumed to have unit length
    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        n = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif
