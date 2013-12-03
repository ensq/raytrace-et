#include <stdafx.h>

#include <Obj.h>
#include <BufSrv.h>
#include <Vertex.h>

Obj::Obj( unsigned p_verticesCnt, unsigned p_indicesCnt, const Vertex* p_vertices, const unsigned* p_indices ) : m_vertices( p_vertices ), m_indices( p_indices ) {
	m_verticesCnt = p_verticesCnt;
	m_indicesCnt = p_indicesCnt;

	m_translation = Mat4F::Identity();
	m_rotation = Mat4F::Identity();
	m_scaling = Mat4F::Identity();

	m_bufferVertex = nullptr;
	m_bufferIndex = nullptr;
}
Obj::~Obj() {
	delete[] m_vertices;
	delete[] m_indices;

	ASSERT_DELETE( m_bufferVertex );
	ASSERT_DELETE( m_bufferIndex );
}

HRESULT Obj::init( ID3D11Device* p_device ) {
	HRESULT hr = S_OK;

	m_bufferVertex = new BufSrv( m_verticesCnt, sizeof( Vertex ), (void*)m_vertices );
	hr = m_bufferVertex->init( p_device );

	if( SUCCEEDED( hr )==true ) {
		m_bufferIndex = new BufSrv( m_indicesCnt, sizeof( unsigned ), (void*)m_indices );
		hr = m_bufferIndex->init( p_device );
	}
	return hr;
}

BufSrv* Obj::getBufferVertex() const {
	return m_bufferVertex;
}
BufSrv* Obj::getBufferIndex() const {
	return m_bufferIndex;
}