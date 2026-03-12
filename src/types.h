#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

enum ColorMode { CYAN, PURPLE, GOLD, WHITE, NUM_MODES };

struct Point3D {
    float x, y, z;
};

struct Point2D {
    int x, y;
    float z; // For Z-sorting and shading
    bool valid;
};

struct Triangle {
    int v[3]; // Indices of vertices
    float avgZ;
    bool visible;
};

struct Particle {
    float x, y;
    float speed;
    uint8_t brightness;
};

#endif
