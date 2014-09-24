#include <stdafx.h>
#include <geometry.h>

Vertex::Vertex() {
    pos = Vec3F(0.0f, 0.0f, 0.0f);
    nor = Vec3F(0.0f, 0.0f, 0.0f);
    tex = Vec2F(0.0f, 0.0f);
}
Vertex::Vertex(Vec3F p_pos, Vec3F p_nor, Vec2F p_tex) {
    pos = p_pos;
    nor = p_nor;
    tex = p_tex;
}
Vertex::~Vertex() {
}

BBox::BBox() {
    min = Vec3F(INFI_FLOAT, INFI_FLOAT, INFI_FLOAT);
    max = Vec3F(-INFI_FLOAT, -INFI_FLOAT, -INFI_FLOAT);
}
BBox::BBox(const Vec3F& p_vu) : min(p_vu), max(p_vu) {
}
BBox::BBox(const Vec3F& p_vl, const Vec3F& p_vr) {
    min = p_vl.min(p_vr);
    max = p_vl.max(p_vr);
}
BBox::BBox(const BBox& p_b, const Vec3F& p_v) {
    min = p_v.min(p_b.min);
    max = p_v.max(p_b.max);
}
BBox::BBox(const BBox& p_bl, const BBox& p_br) {
    Vec3F br_min = p_br.min;
    Vec3F br_max = p_br.max;

    min = br_min.min(p_bl.min);
    max = br_max.max(p_bl.max);
}
BBox::~BBox() {
}

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
int BBox::getAxisIdxMax() const {
    Vec3F span = max - min;
    if(span.x>span.y && span.x>span.z) {
        return AXIS_X;
    } else if(span.y>span.z) {
        return AXIS_Y;
    }
    return AXIS_Z;
}
int BBox::getAxisIdxMin() const {
    Vec3F span = max - min;
    if(span.x<span.y && span.x<span.z) {
        return AXIS_X;
    } else if(span.y<span.z) {
        return AXIS_Y;
    }
    return AXIS_Z;
}
