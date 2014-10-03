#ifndef DV2520_ANT_H
#define DV2520_ANT_H

#include <Singleton.h> // Included for simplicity when initializing
                       // Ant as a singleton.

class Ant {
  public:
    Ant();
    ~Ant();

    void init(ID3D11Device* p_device, unsigned p_screenWidth,
              unsigned p_screenHeight);
    void render();
    int eventWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    double getTimeRaysGenerate() const;
    double getTimeRaysIntersect() const;
    double getTimeLighting() const;

    void setTimeRaysGenerate(double p_time);
    void setTimeRaysInterect(double p_time);
    void setTimeLighting(double p_time);
    void setEyeFixation(double p_x, double p_y);
    void setPos(float p_x, float p_y, float p_z);
    void setLook(float p_x, float p_y, float p_z);
  protected:
  private:
    double m_timeRaysGenerate;
    double m_timeRaysInterect;
    double m_timeLighting;

    double m_eyeFixPosX;
    double m_eyeFixPosY;

    float m_posX;
    float m_posY;
    float m_posZ;

    float m_lookX;
    float m_lookY;
    float m_lookZ;
};

#endif DV2520_ANT_H
