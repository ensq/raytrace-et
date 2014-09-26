#ifndef DV2520_FOV_H
#define DV2520_FOV_H

#define BLOCK_SIZE 16

typedef long HRESULT;

class BufUav;
class TimerD3d;
class FovTarget;

class CogFx;
class CogCb;

// Foveated View
class Fov {
 public:
    Fov(unsigned p_width, unsigned p_height,
        unsigned p_ofsX, unsigned p_ofsY,
        ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);
    ~Fov();

    HRESULT init();

    void render(CogFx* p_cogFx, CogCb* p_cogCb);

// temp:
FovTarget* getFovTarget() {return m_target;};
 protected:
 private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_ofsX;
    unsigned m_ofsY;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_devcon;
    
    BufUav* m_uavRays;
    BufUav* m_uavIntersections;
    BufUav* m_uavColor;

    FovTarget* m_target;
};

#endif // DV2520_FOV_H
