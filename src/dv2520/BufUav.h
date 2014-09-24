#ifndef DV2520_BUFUAV_H
#define DV2520_BUFUAV_H

#include <Buf.h>

class BufUav : public Buf {
  public:
    BufUav(unsigned p_tNum, unsigned p_tSize, DXGI_FORMAT p_tFormat);
    virtual ~BufUav();

    HRESULT init(ID3D11Device* p_device);

    ID3D11UnorderedAccessView* getUav() const;
  protected:
  private:
    HRESULT initBuf(ID3D11Device* p_device);
    HRESULT initUav(ID3D11Device* p_device);

    DXGI_FORMAT m_tFormat;
    ID3D11UnorderedAccessView* m_uav;
};

#endif // DV2520_BUFUAV_H