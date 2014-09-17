#ifndef DV2520_GEOMETRY_H
#define DV2520_GEOMETRY_H

#include <MathSimple.h>

struct Vertex {
    Vec3F pos;
    Vec3F nor;
    Vec2F tex;

    Vertex();
    Vertex(Vec3F p_pos, Vec3F p_nor, Vec2F p_tex);
    ~Vertex();
};

struct BBox {
    Vec3F min;
    Vec3F max;
    
    BBox();
    BBox(const Vec3F& p_vu);
    BBox(const Vec3F& p_vl, const Vec3F& p_vr);
    BBox(const BBox& p_b, const Vec3F& p_v);
    BBox(const BBox& p_bl, const BBox& p_br);
    ~BBox();


    int getAxisIdxMin() const;
    int getAxisIdxMax() const;
};

#endif // DV2520_GEOMETRY_H
