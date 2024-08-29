#include <iostream>
#include <cmath>
#include <cstdint>

// Simple hash function to generate pseudorandom gradients
uint8_t random3(int x, int y, int z, uint32_t seed) {
    uint32_t n = (x * 374761393 + y * 668265263 + z * 9301) ^ (seed * 982451653);
    n = (n ^ (n >> 13)) * 1274126177;
    n ^= n<<5;
    n ^= n>>3;
    n ^= n>>4;
    return static_cast<uint8_t>((n ^ (n >> 16)) % 256);
}

// Fade function for smoothing (with uint8_t precision)
uint8_t fade(uint8_t t) {
    float tf = t / 255.0f;  // Convert to float in range [0, 1]
    float fadeValue = tf * tf * tf * (tf * (tf * 6 - 15) + 10);
    return static_cast<uint8_t>(fadeValue * 255);  // Scale back to [0, 255]
}

// Linear interpolation for uint8_t
uint8_t lerp(uint8_t a, uint8_t b, uint8_t t) {
    return a + (t * (b - a)) / 255;  // Integer math scaled by 255
}

// Dot product of distance and gradient vectors, scaled for uint8_t
uint8_t dotGridGradient(int ix, int iy, int iz, float x, float y, float z, uint32_t seed) {
    // Generate gradient vector
    float gradientX = (random3(ix, iy, iz, seed) / 255.0f) * 2 - 1; // Range [-1, 1]
    float gradientY = (random3(ix + 1, iy, iz, seed) / 255.0f) * 2 - 1;
    float gradientZ = (random3(ix, iy + 1, iz, seed) / 255.0f) * 2 - 1;

    // Compute the distance vector
    float dx = x - static_cast<float>(ix);  // Scale to float coordinates
    float dy = y - static_cast<float>(iy);
    float dz = z - static_cast<float>(iz);

    // Compute the dot product, then scale to range [0, 255]
    float dot = (dx * gradientX + dy * gradientY + dz * gradientZ);
    return static_cast<uint8_t>(std::fmax(0.0f, std::fmin(255.0f, 128.0f + dot * 128.0f)));  // Adjust scale for 0-255
}

// 3D Perlin noise function using uint8_t
uint8_t perlin(int32_t x, int32_t y = 0, int32_t z = 0, uint32_t seed = 0, float scale = 0.05f) {
    // Scale x, y, and z to increase frequency (adjust this scale for smoother results)
    float fx = x * scale;
    float fy = y * scale;
    float fz = z * scale;

    // Determine grid cell coordinates
    int x0 = static_cast<int>(std::floor(fx));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(std::floor(fy));
    int y1 = y0 + 1;
    int z0 = static_cast<int>(std::floor(fz));
    int z1 = z0 + 1;

    // Interpolation weights
    uint8_t sx = fade(static_cast<uint8_t>((fx - x0) * 255)); // Convert float diff to [0, 255] scale
    uint8_t sy = fade(static_cast<uint8_t>((fy - y0) * 255));
    uint8_t sz = fade(static_cast<uint8_t>((fz - z0) * 255));

    // Dot products at each corner of the cube
    uint8_t n000 = dotGridGradient(x0, y0, z0, fx, fy, fz, seed);
    uint8_t n100 = dotGridGradient(x1, y0, z0, fx, fy, fz, seed);
    uint8_t n010 = dotGridGradient(x0, y1, z0, fx, fy, fz, seed);
    uint8_t n110 = dotGridGradient(x1, y1, z0, fx, fy, fz, seed);

    uint8_t n001 = dotGridGradient(x0, y0, z1, fx, fy, fz, seed);
    uint8_t n101 = dotGridGradient(x1, y0, z1, fx, fy, fz, seed);
    uint8_t n011 = dotGridGradient(x0, y1, z1, fx, fy, fz, seed);
    uint8_t n111 = dotGridGradient(x1, y1, z1, fx, fy, fz, seed);

    // Interpolate along x
    uint8_t ix00 = lerp(n000, n100, sx);
    uint8_t ix10 = lerp(n010, n110, sx);
    uint8_t ix01 = lerp(n001, n101, sx);
    uint8_t ix11 = lerp(n011, n111, sx);

    // Interpolate along y
    uint8_t iy0 = lerp(ix00, ix10, sy);
    uint8_t iy1 = lerp(ix01, ix11, sy);

    // Final interpolation along z
    return lerp(iy0, iy1, sz);
}