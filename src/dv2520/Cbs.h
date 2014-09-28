#ifndef DV2520_CBS_H
#define DV2520_CBS_H

#include <MathSimple.h>

struct CbPerInstance {
    unsigned screenWidth;
    unsigned screenHeight;
    Vec2F pad;
};

struct CbPerFrame {
    Mat4F view;
    Mat4F viewInv;

    Vec3F pos;
    unsigned instancesCnt;

    unsigned lightsCnt;
    Vec3F pad;
};

struct CbPerFov {
    Mat4F proj;
    Mat4F projInv;
    
    unsigned fovWidth;
    unsigned fovHeight;
    unsigned fovOfsX;
    unsigned fovOfsY;

    float fov;
    float aspect;
    Vec2F pad;
};

#endif // DV2520_CBS_H
