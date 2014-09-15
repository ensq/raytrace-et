#ifndef DV2520_VERTEX_H
#define DV2520_VERTEX_H

#include <MathSimple.h>

struct Vertex {
    Vec3F pos;
    Vec3F nor;
    Vec2F tex;

    Vertex() {
        pos = Vec3F( 0.0f, 0.0f, 0.0f );
        nor = Vec3F( 0.0f, 0.0f, 0.0f );
        tex = Vec2F( 0.0f, 0.0f );
    }
    Vertex( Vec3F p_pos, Vec3F p_nor, Vec2F p_tex ) {
        pos = p_pos;
        nor = p_nor;
        tex = p_tex;
    }
    ~Vertex() {
    }
};

#endif // DV2520_VERTEX_G