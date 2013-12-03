#include <stdafx.h>

#include <ObjFace.h>

ObjFace::ObjFace() {
	m_v = m_t = m_n = m_index = 0;
}
ObjFace::ObjFace( unsigned p_v, unsigned p_t, unsigned p_n, unsigned p_index ) {
	m_v = p_v;
	m_t = p_t;
	m_n = p_n;
	m_index = p_index;
}
ObjFace::~ObjFace() {
}

unsigned ObjFace::getV() const {
	return m_v;
}
unsigned ObjFace::getT() const {
	return m_t;
}
unsigned ObjFace::getN() const {
	return m_n;
}
unsigned ObjFace::getIndex() const {
	return m_index;
}
void ObjFace::setIndex( unsigned p_index ) {
	m_index = p_index;
}