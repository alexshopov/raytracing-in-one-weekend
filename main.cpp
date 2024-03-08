#include <iostream>

#include "src/ray-tracing-weekend.h"
#include "src/camera.h"
#include "src/hittable-list.h"
#include "src/sphere.h"

int main(int, char**){
    hittable_list world;
    world.add(make_shared<sphere>(point3{0, 0, -1}, 0.5));
    world.add(make_shared<sphere>(point3{0, -100.5, -1}, 100));

    camera cam;
    cam.aspect_ratio = 16./9.;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}
