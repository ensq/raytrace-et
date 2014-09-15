#ifndef DV2520_RAY_H
#define DV2520_RAY_H

#include <MathSimple.h>

struct Ray {
    Vec3F pos;
    float distMin;

    Vec3F dir;
    float distMax;

    Vec3F light;
    int primID; // a unique identifier to the corresponding index of a mesh in a buffer of meshes, not necessarily the index in-and-of-it's-own.
};

#endif // DV2520_RAY_H