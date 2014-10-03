#ifndef DV2520_CAM_H
#define DV2520_CAM_H

#include <MathSimple.h>

class Cam {
  public:
    Cam(Vec3F p_pos, Vec3F p_look, float p_zNear, float p_zFar);
    ~Cam();

    void getView(Mat4F& io_view);
    void getProj(float p_fov, float p_aspect,
                 Mat4F& io_proj);
    
    void strafe(const float p_velocity);
    void walk(const float p_velocity);
    void pitch(const float p_angle);
    void yaw(const float p_angle);

    Vec3F getPos() const;
    Vec3F getLook() const;
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
