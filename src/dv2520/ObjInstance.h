#ifndef DV2520_OBJINSTANCE_H
#define DV2520_OBJINSTANCE_H

#include <MathSimple.h> // eww.

struct ObjInstance {
    Mat4F world;
    Mat4F worldInv;

    unsigned indexStart;
    unsigned indexCnt;
    unsigned vertexCnt;
    unsigned pad;
};

#endif // DV2520_OBJINSTANCE_H