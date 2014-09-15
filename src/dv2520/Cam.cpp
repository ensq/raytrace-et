#include <stdafx.h>

#include <Cam.h>

Cam::Cam( float p_fov, float p_aspect, float p_zNear, float p_zFar ) {
    m_fov = p_fov;
    m_aspect = p_aspect;
    m_zNear = p_zNear;
    m_zFar = p_zFar;

    m_pos = Vec3F( 0.0f, 0.0f, -45.0f );
    m_right = Vec3F( 1.0f, 0.0f, 0.0f );
    m_up = Vec3F( 0.0f, 1.0f, 0.0f );
    m_look = Vec3F( 0.0f, 0.0f, 1.0f );
}
Cam::~Cam() {
}

void Cam::update( double p_delta ) {
    updateView();
    updateProj();
}

void Cam::strafe( const float p_velocity ) {
    m_pos += m_right * p_velocity;
}
void Cam::walk( const float p_velocity ) {
    m_pos += m_look * p_velocity;
}
void Cam::pitch( const float p_angle ) {
    m_up.rotate( p_angle, m_right );
    m_look.rotate( p_angle, m_right );
}
void Cam::yaw( const float p_angle ) {
    Vec3F axisY = Vec3F( 0.0f, 1.0f, 0.0f );
    m_right.rotate( p_angle, axisY );
    m_up.rotate( p_angle, axisY );
    m_look.rotate( p_angle, axisY );
}

Vec3F Cam::getPos() const {
    return m_pos;
}
Mat4F Cam::getView() const {
    return m_view;
}
Mat4F Cam::getProj() const {
    return m_proj;
}

void Cam::updateView() {
    m_look.normalize();

    m_up = m_look.cross( m_right );
    m_up.normalize();

    m_right = m_up.cross( m_look );
    m_right.normalize();

    float x = -m_pos.dot( m_right );
    float y = -m_pos.dot( m_up );
    float z = -m_pos.dot( m_look );

    m_view._[ 0 ][ 0 ] = m_right.x;
    m_view._[ 1 ][ 0 ] = m_right.y;
    m_view._[ 2 ][ 0 ] = m_right.z;
    m_view._[ 3 ][ 0 ] = x;
                        
    m_view._[ 0 ][ 1 ] = m_up.x;
    m_view._[ 1 ][ 1 ] = m_up.y;
    m_view._[ 2 ][ 1 ] = m_up.z;
    m_view._[ 3 ][ 1 ] = y;
                        
    m_view._[ 0 ][ 2 ] = m_look.x;
    m_view._[ 1 ][ 2 ] = m_look.y;
    m_view._[ 2 ][ 2 ] = m_look.z;
    m_view._[ 3 ][ 2 ] = z;
                        
    m_view._[ 0 ][ 3 ] = 0.0f;
    m_view._[ 1 ][ 3 ] = 0.0f;
    m_view._[ 2 ][ 3 ] = 0.0f;
    m_view._[ 3 ][ 3 ] = 1.0f;
}
void Cam::updateProj() {
    Mat4F perspective; ZERO_MEM( perspective );
    perspective._[ 0 ][ 0 ] = 1 / ( m_aspect * ( tan( m_fov / 2 ) ) );
    perspective._[ 1 ][ 1 ] = 1 / ( tan( m_fov / 2 ) );
    perspective._[ 2 ][ 2 ] = m_zFar / ( m_zFar - m_zNear );
    perspective._[ 2 ][ 3 ] = 1.0f;
    perspective._[ 3 ][ 2 ] = ( -m_zNear * m_zFar ) / ( m_zFar - m_zNear );
    m_proj = perspective;
}