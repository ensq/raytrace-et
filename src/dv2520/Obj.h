#ifndef DV2520_OBJ_H
#define DV2520_OBJ_H

struct Vertex;

class BufSrv;

#include <MathSimple.h>

class Obj {
public:
	Obj( unsigned p_verticesCnt, unsigned p_indicesCnt, const Vertex* p_vertices, const unsigned* p_indices );
	~Obj();

	HRESULT init( ID3D11Device* p_device );

	BufSrv* getBufferVertex() const;
	BufSrv* getBufferIndex() const;
protected:
private:
	unsigned m_verticesCnt;
	unsigned m_indicesCnt;

	const Vertex* m_vertices;
	const unsigned* m_indices;

	BufSrv* m_bufferVertex;
	BufSrv* m_bufferIndex;

	Mat4F m_translation;
	Mat4F m_rotation;
	Mat4F m_scaling;
};

#endif // DV2520_OBJ_H