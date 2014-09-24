#ifndef DV2520_INPUTQUEUE_H
#define DV2520_INPUTQUEUE_H

#include <Input.h>

class InputQueue {
  public:
    InputQueue();
    ~InputQueue();

    void keyPush(InputKey p_key);
    bool keyPop(InputKey& io_key);

    void mousePush(InputMouse p_mouse);
    void mousePop(InputMouse& io_mouse);

    void empty();
  protected:
  private:
    std::queue<InputKey> m_keys;
    InputMouse m_mouseCur;
    InputMouse m_mousePrev;
};

#endif // DV2520_INPUTQUEUE_H