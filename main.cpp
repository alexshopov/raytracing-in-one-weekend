#include <iostream>
#include <fstream>

void render(int width, int height) {
    std::ofstream out;
    out.open("out.ppm");

    out << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; ++j) {
        std::clog << "\nScanlines remaining: " << (height - j) << ' ' << std::flush;
        for (int i = 0; i < width; ++i) {
            auto r = double(i) / (width - 1);
            auto g = double(j) / (height - 1);
            auto b = 0.5;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            out << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    out.close();
    std::clog << "\nDone.          \n";
}

int main(int, char**){
    // image properties
    int image_width = 256;
    int image_height = 256;

    render(image_width, image_height);
}
