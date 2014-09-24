#ifndef DV2520_FXCS_H
#define DV2520_FXCS_H

#include <Fx.h>

class FxCs : public Fx {
  public:
    FxCs(LPCWSTR p_kernelPath);
    virtual ~FxCs();

    HRESULT init(ID3D11Device* p_device);

    void set(ID3D11DeviceContext* p_devcon);
    void unset(ID3D11DeviceContext* p_devcon);
  protected:
  private:
    ID3D11ComputeShader* m_cs;
};

#endif // DV2520_FXCS_H