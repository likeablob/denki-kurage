#ifndef MATH_3D_H
#define MATH_3D_H

#include "config.h"
#include "types.h"

void updateRotationParams(float ax, float ay, float az);
Point3D rotateFast(Point3D p);
Point2D project(Point3D p, float global_x, float global_y, float global_z);

#endif
