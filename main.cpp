#include <iostream>
#include <stdio.h>
#include <fstream>
#include <chrono>
#include "noise.h" //perlin(int32_t x, int32_t y, int32_t z, uint32_t seed, float scale)

int main() {
	std::ofstream file("out.txt");
    uint32_t seed = 3422365;  // Example seed
    uint32_t width = 128;
    uint32_t height = 128;
    file << (uint32_t)width << " " << (uint32_t)height << " ";
    for (int32_t x = 0; x < width; x++) {
    	for (int32_t y = 0; y < height; y++) {
    		uint32_t value = perlin(x, y, 0, seed, 0.05f);
    		uint32_t flat = perlin(x,y,0, seed, 0.1f);
            //value += perlin(20*x, 20*y, 1, seed);
            //value += perlin(3*x, 3*y, 1, seed);
            //value += perlin(4*x, 4*y, 1, seed);
            //value /= 3;
    		float biome = flat / 256.0f;
    		value *= biome;
    		value /= 3;

    		file << (uint32_t)(value) << " ";
    	}
	    //file << std::endl;
    }
    
    file.close();
    std::cout << "done";
    return 0;
}