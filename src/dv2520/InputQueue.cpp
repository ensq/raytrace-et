#include <stdafx.h>

#include <InputQueue.h>

InputQueue::InputQueue() {
}
InputQueue::~InputQueue() {
}

void InputQueue::keyPush( InputKey p_key ) {
	m_keys.push( p_key );
}
bool InputQueue::keyPop( InputKey& io_key ) {
	bool empty = true;
	if( m_keys.empty()==false ) {
		io_key = m_keys.front();
		m_keys.pop();
		empty = false;
	}
	return empty;
}

void InputQueue::empty() {
	while( m_keys.empty()==false ) {
		m_keys.pop();
	}
	m_mouseCur = InputMouse();
}

void InputQueue::mousePush( InputMouse p_mouse ) {
#define mousePixelToAngle 1.0f
	float dx = RADIAN( mousePixelToAngle * static_cast< float >( p_mouse.getDX() - m_mousePrev.getDX() ) );
	float dy = RADIAN( mousePixelToAngle * static_cast< float >( p_mouse.getDY() - m_mousePrev.getDY() ) );
	m_mouseCur = InputMouse( dx, dy );
	m_mousePrev = InputMouse( p_mouse.getDX(), p_mouse.getDY() );
}
void InputQueue::mousePop( InputMouse& io_mouse ) {
	io_mouse = m_mouseCur;
}