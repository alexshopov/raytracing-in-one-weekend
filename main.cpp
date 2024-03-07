#include <iostream>
#include <fstream>

#include "color.h"
#include "vec3.h"

void render(int width, int height) {
}

int main(int, char**){
    // image properties
    int image_width = 256;
    int image_height = 256;

    std::ofstream out;
    out.open("out.ppm");
    if (!out.is_open()) {
        std::clog << "Error: could not open output file" << std::endl;
        exit(1);
    }

    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\nScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {
            auto pixel_color = color(double(i) / (image_width - 1),
                                     double(j) / (image_height - 1),
                                     0);
            write_color(out, pixel_color);
        }
    }

    std::clog << "\nDone.          \n";

    out.close();
}
