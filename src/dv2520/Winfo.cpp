#include <stdafx.h>

#include <Winfo.h>

Winfo::Winfo( 
	HINSTANCE p_hInstance, 
	HINSTANCE p_hInstancePrev, 
	LPWSTR p_lpCmdLine, 
	int p_nCmdShow ) {
		m_hInstance		= p_hInstance;
		m_hInstancePrev = p_hInstancePrev;
		m_lpCmdLine		= p_lpCmdLine;
		m_nCmdShow		= p_nCmdShow;
}
Winfo::~Winfo() {
}

HINSTANCE Winfo::getHInstance() const {
	return m_hInstance;
}
HINSTANCE Winfo::getHInstancePrev() const {
	return m_hInstancePrev;
}
LPWSTR Winfo::getLpCmdLine() const {
	return m_lpCmdLine;
}
int Winfo::getNCmdShow() const {
	return m_nCmdShow;
}