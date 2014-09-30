#ifndef DV2520_FOV_H
#define DV2520_FOV_H

#define BLOCK_SIZE 16

typedef long HRESULT;

class Cam;
class BufUav;
class TimerD3d;
class FovTarget;

class CogFx;
class CogCb;

struct DescFov {
    unsigned width;
    unsigned height;
    unsigned widthUpscale;
    unsigned heightUpscale;
    unsigned ofsX;
    unsigned ofsY;
    float fov;
    float aspect;
};

// Foveated View
class Fov {
 public:
    Fov(DescFov p_desc,
        ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);
    ~Fov();

    HRESULT init();

    void renderToFov(CogFx* p_cogFx, CogCb* p_cogCb,
                     Cam* p_cam, bool p_isOffset);
    void renderToBackbuffer(CogFx* p_cogFx, CogCb* p_cogCb,
                            ID3D11UnorderedAccessView* p_uavBackbuffer);

    void setOfs(unsigned p_x, unsigned p_y) {
        m_ofsX = p_x;
        m_ofsY = p_y;
    }
 protected:
 private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_widthUpscale;
    unsigned m_heightUpscale;
    unsigned m_ofsX;
    unsigned m_ofsY;
    float m_fov;
    float m_aspect;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_devcon;
    
    BufUav* m_uavRays;
    BufUav* m_uavIntersections;
    BufUav* m_uavColor;

    FovTarget* m_target;
};

#endif // DV2520_FOV_H
