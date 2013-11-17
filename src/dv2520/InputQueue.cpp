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

void InputQueue::mousePush( InputMouse p_mouse ) {
	InputMouse mouse( m_mouse.getDX() + p_mouse.getDX(), m_mouse.getDY() + p_mouse.getDY() );
	m_mouse = mouse;
}
InputMouse InputQueue::mousePop() {
	InputMouse mouse = m_mouse;
	m_mouse = InputMouse();
	return mouse;
}

void InputQueue::empty() {
	while( m_keys.empty()==false ) {
		m_keys.pop();
	}
	m_mouse = InputMouse();
}