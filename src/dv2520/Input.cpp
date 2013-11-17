#include <stdafx.h>

#include <Input.h>

InputKey::InputKey() {
	m_type = InputKeyTypes_NA;
}
InputKey::InputKey( InputKeyTypes p_type ) {
	m_type = p_type;
}
InputKey::~InputKey() {
}

bool InputKey::isValid() const {
	bool isValid = true;
	if( m_type==InputKeyTypes_NA ) {
		isValid = false;
	}
	return isValid;
}

InputKeyTypes InputKey::getType() const {
	return m_type;
}

InputMouse::InputMouse() {
	m_dX = 0.0;
	m_dY = 0.0;
}
InputMouse::InputMouse( double p_dX, double p_dY ) {
	m_dX = p_dX;
	m_dY = p_dY;
}
InputMouse::~InputMouse() {
}

double InputMouse::getDX() const {
	return m_dX;
}
double InputMouse::getDY() const {
	return m_dY;
}