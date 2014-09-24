#ifndef DV2549_EXPERIMENT_TIMERD3D_H
#define DV2549_EXPERIMENT_TIMERD3D_H

struct ID3D11Query;
struct ID3D11Device;
struct ID3D11DeviceContext;

class TimerD3d {
  public:
    TimerD3d();
    ~TimerD3d();

    HRESULT init(ID3D11Device* p_device);

    void start(ID3D11DeviceContext* p_devcon);
    void stop(ID3D11DeviceContext* p_devcon);
    double time(ID3D11DeviceContext* p_devcon);
  protected:
  private:
    ID3D11Query* m_start;
    ID3D11Query* m_stop;
    ID3D11Query* m_disjoint;
};

#endif // DV2549_EXPERIMENT_TIMERD3D_H