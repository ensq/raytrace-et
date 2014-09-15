#ifndef DV2520_OBJ_H
#define DV2520_OBJ_H

struct Vertex;

#include <MathSimple.h>

class Obj {
public:
    Obj( unsigned p_verticesCnt, unsigned p_indicesCnt, const Vertex* p_vertices, const unsigned* p_indices );
    ~Obj();

    unsigned getVerticesCnt() const;
    unsigned getIndicesCnt() const;
    Vertex* getVertices() const;
    unsigned* getIndices() const;

    Mat4F& getTranslation();
    Mat4F& getRotation();
    Mat4F& getScaling();
    Mat4F getWorldTransform() const;
protected:
private:
    unsigned m_verticesCnt;
    unsigned m_indicesCnt;

    Vertex* m_vertices;
    unsigned* m_indices;
    Mat4F m_translation;
    Mat4F m_rotation;
    Mat4F m_scaling;
};

#endif // DV2520_OBJ_H