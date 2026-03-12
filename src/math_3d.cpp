#include "math_3d.h"
#include <math.h>

static float sx, cx, sy, cy, sz, cz;
static const float fov = 160.0f;
static const float camera_dist = 500.0f;

void updateRotationParams(float ax, float ay, float az) {
    sx = sinf(ax);
    cx = cosf(ax);
    sy = sinf(ay);
    cy = cosf(ay);
    sz = sinf(az);
    cz = cosf(az);
}

Point3D rotateFast(Point3D p) {
    Point3D res = p;
    float tmp_y, tmp_z, tmp_x;
    tmp_y = res.y * cx - res.z * sx;
    tmp_z = res.y * sx + res.z * cx;
    res.y = tmp_y;
    res.z = tmp_z;
    tmp_x = res.x * cy + res.z * sy;
    tmp_z = -res.x * sy + res.z * cy;
    res.x = tmp_x;
    res.z = tmp_z;
    tmp_x = res.x * cz - res.y * sz;
    tmp_y = res.x * sz + res.y * cz;
    res.x = tmp_x;
    res.y = tmp_y;
    return res;
}

Point2D project(Point3D p, float global_x, float global_y, float global_z) {
    Point2D p2;
    float world_z = camera_dist + p.z + global_z;
    if(world_z < 20.0f) {
        p2.valid = false;
        return p2;
    }
    float scale = fov / world_z;
    p2.x = (int)(SCREEN_WIDTH / 2 + (p.x + global_x) * scale);
    p2.y = (int)(SCREEN_HEIGHT / 2 + (p.y + global_y) * scale);
    p2.z = world_z; // Store depth
    p2.valid = (p2.x >= -50 && p2.x < SCREEN_WIDTH + 50 && p2.y >= -50 &&
                p2.y < SCREEN_HEIGHT + 50);
    return p2;
}
