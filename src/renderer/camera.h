#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <fstream>

#include "../ray-tracing-in-one-weekend.h"
#include "hittable.h"
#include "../mats/color.h"
#include "../mats/material.h"

class camera {
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2. * h * focus_dist;
        auto viewport_width = viewport_height
            * (static_cast<double>(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(vup.cross(w));
        v = w.cross(u);

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // camera defocus disk basis vectors
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2.));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    color ray_color(const ray &r, int depth, const hittable &world) const {
        hit_record rec;

        if (depth <= 0.)
            return COLOR_BLACK;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attentuation;
            if (rec.mat->scatter(r, rec, attentuation, scattered))
                return attentuation * ray_color(scattered, depth - 1, world);
            return COLOR_BLACK;
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y + 1.);
        return (1. - a) * COLOR_WHITE + a * color{0.5, 0.7, 1.};
    }

    /**
     * Returns a random point in the camera defocus disk
     */
    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    ray get_ray(int i, int j) const {
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = defocus_angle <= 0 ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

public:
    double aspect_ratio = 1.;
    int image_width = 100;      // rendered image width
    int samples_per_pixel = 10; // count of random samples for each pixel
    int max_depth = 10;         // max numnber of ray bounces into scene

    double vfov     = 90.;              // vertical view angle
    point3 lookfrom = point3{0, 0, -1}; // point camera is looking fomr
    point3 lookat   = point3{0, 0, 0};  // point camera is looking at
    vec3   vup      = vec3{0, 1, 0};    // camera up vector

    double defocus_angle = 0; // variation angle of rays through each pixel
    double focus_dist = 10;   // distance from camera lookfrom point to focul plane

    void render(const hittable &world) {
        initialize();

        // open output file
        //--------------------------------------------------------------
        std::ofstream out;
        out.open("out.ppm");
        if (!out.is_open()) {
            std::clog << "Error: could not open output file" << std::endl;
            exit(1);
        }

        // render
        //--------------------------------------------------------------
        out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\nScanlines remaining: " << (image_height - j) << ' ' << std::flush;

            for (int i = 0; i < image_width; ++i) {
                color pixel_color{0, 0, 0};
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(out, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\nDone.          \n";

        // clean up
        //--------------------------------------------------------------
        out.close();
    }
};

#endif
