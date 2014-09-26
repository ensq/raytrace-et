#ifndef DV2520_CBS_H
#define DV2520_CBS_H

#include <MathSimple.h>

struct CbPerInstance {
    unsigned screenWidth;
    unsigned screenHeight;
    float aspect;
    float fov;
};

struct CbPerFrame {
    Mat4F view;
    Mat4F viewInv;
    Mat4F proj;
    Mat4F projInv;

    Vec3F pos;
    unsigned instancesCnt;

    unsigned lightsCnt;
    Vec3F pad;
};

struct CbPerFov {
    unsigned fovWidth;
    unsigned fovHeight;
    unsigned fovOfsX;
    unsigned fovOfsY;
};

#endif // DV2520_CBS_H
