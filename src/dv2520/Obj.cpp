#include <stdafx.h>

#include <Obj.h>
#include <geometry.h>

Obj::Obj( unsigned p_verticesCnt, unsigned p_indicesCnt, const Vertex* p_vertices, const unsigned* p_indices ) {
    m_verticesCnt = p_verticesCnt;
    m_indicesCnt = p_indicesCnt;

    // Allocate space for unique copy of vertices and indices for Object:
    m_vertices = (Vertex*)calloc( m_verticesCnt, sizeof( Vertex ) );
    m_indices = (unsigned*)calloc( m_indicesCnt, sizeof( unsigned ) );
    // Copy vertices and indices to newly allocated space:
    memcpy( m_vertices, p_vertices, sizeof( Vertex ) * m_verticesCnt );
    memcpy( m_indices, p_indices, sizeof( unsigned ) * m_indicesCnt );

    m_translation = Mat4F::Identity();
    m_rotation = Mat4F::Identity();
    m_scaling = Mat4F::Identity();

    // Test:
    m_scaling.scale( 1.0, 1.0f, 1.0f );
    m_translation.translate( 0.0f, 0.0f, 0.0f );
    m_rotation.rotate( 0.0f, 0.0f, 0.0f );
}
Obj::~Obj() {
    free( m_vertices );
    free( m_indices );
}

unsigned Obj::getVerticesCnt() const {
    return m_verticesCnt;
}
unsigned Obj::getIndicesCnt() const {
    return m_indicesCnt;
}
Vertex* Obj::getVertices() const {
    return m_vertices;
}
unsigned* Obj::getIndices() const {
    return m_indices;
}

Mat4F& Obj::getTranslation() {
    return m_translation;
}
Mat4F& Obj::getRotation() {
    return m_rotation;
}
Mat4F& Obj::getScaling() {
    return m_scaling;
}
Mat4F Obj::getWorldTransform() const {
    return ( m_scaling * m_rotation ) * m_translation;
}