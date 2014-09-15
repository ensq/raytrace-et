#ifndef DV2520_INTERSECTION_H
#define DV2520_INTERSECTION_H

struct Intersection {
    float u;
    float v;
    float t;

    int primId;
    unsigned primVertexOffset;
    unsigned instanceIdx;

    float dist;
};

#endif // DV2520_INTERSECTION_H