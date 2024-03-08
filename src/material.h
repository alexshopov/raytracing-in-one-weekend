#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray-tracing-in-one-weekend.h"
#include "color.h"

class hit_record;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in,
                         const hit_record &rec,
                         color &attenuation,
                         ray &scattered) const = 0;
};

class lambertian : public material {
    color albedo;

public:
    lambertian(const color &a) : albedo{a} {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        auto scatter_direction = rec.n + random_unit_vector();

        // catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.n;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class metal : public material {
    color albedo;

public:
    metal(const color &a) : albedo{a} {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.n);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }
};

#endif
