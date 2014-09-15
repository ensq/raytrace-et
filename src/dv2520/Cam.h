#ifndef DV2520_CAM_H
#define DV2520_CAM_H

#include <MathSimple.h>

class Cam {
public:
    Cam( float p_fov, float p_aspect, float p_zNear, float p_zFar );
    ~Cam();

    void update( double p_delta );

    void strafe( const float p_velocity );
    void walk( const float p_velocity );
    void pitch( const float p_angle );
    void yaw( const float p_angle );

    Vec3F getPos() const;
    Mat4F getView() const;
    Mat4F getProj() const;
protected:
private:
    void updateView();
    void updateProj();

    float m_fov;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    Vec3F m_pos;
    Vec3F m_right;
    Vec3F m_up;
    Vec3F m_look;

    Mat4F m_view;
    Mat4F m_proj;
};

#endif // DV2520_CAM_H