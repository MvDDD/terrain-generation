#include <iostream>
#include <fstream>
#include <stdexcept> // For std::invalid_argument
#include <string>
#include "noise.h" // Ensure this file is present and contains the correct function signature

int main(int argc, char *argv[]) {
    uint32_t width, height;
    try {
        width = 2048;
        height = width; // Assuming square dimensions as per your original code
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: Invalid argument. Please enter a valid integer for width/height.\n";
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << "Error: Argument out of range. Please enter a smaller integer.\n";
        return 1;
    }

    std::ofstream file("out.txt");
    uint32_t seed = 4938765;
    file << width << " " << height << " ";

    for (int32_t x = 0; x < width; x++) {
        for (int32_t y = 0; y < height; y++) {
            uint32_t value = perlin(x, y, 0, seed, 0.05f);
            uint32_t flat = perlin(x, y, 0, seed, 0.005f);
            value += perlin(20 * x, 20 * y, 0, seed, 0.05f);
            value += perlin(3 * x, 3 * y, 0, seed, 0.05f);
            value += perlin(4 * x, 4 * y, 0, seed, 0.05f);
            value /= 3;
            float biome = flat / 256.0f;
            value *= biome;
            value /= 0 + 5;

            file << (uint32_t)(value) << " ";
        }
        file << std::endl;
    }

    file.close();
    std::cout << "done\n";
    return 0;
}
