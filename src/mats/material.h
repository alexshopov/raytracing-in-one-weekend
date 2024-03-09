#ifndef MATERIAL_H
#define MATERIAL_H

#include "../ray-tracing-in-one-weekend.h"
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
    double fuzz;

public:
    metal(const color &a, double f) : albedo{a}, fuzz{f} {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.n);
        scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
        attenuation = albedo;
        return scattered.direction().dot(rec.n) > 0;
    }
};

class dielectric : public material {
    double ir; // index of refraction

    static double reflectance(double cosine, double ref_idx) {
        // Schlick approximation for reflectance
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 *= r0;
        return r0 + (1 - r0) * pow(1 - cosine, 5);
    }

public:
    dielectric(double index_of_refraction) : ir{index_of_refraction} {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        attenuation = color{1., 1., 1.};
        double refraction_ratio = rec.front_face ? (1. / ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(-unit_direction.dot(rec.n), 1.);
        double sin_theta = sqrt(1. - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.n);
        else
            direction = refract(unit_direction, rec.n, refraction_ratio);

        scattered = ray(rec.p, direction);
        return true;
    }
};

#endif
