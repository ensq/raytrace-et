#include <stdafx.h>

#include <ExceptionDv2520.h>

ExceptionDv2520::ExceptionDv2520( std::string p_func ) throw() {
	m_func = p_func;
}
ExceptionDv2520::~ExceptionDv2520() throw() {
}

const char* ExceptionDv2520::what() const throw() {
	std::string what = "Dv2520: " + m_func;
	return what.c_str();
}