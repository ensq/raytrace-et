#ifndef DV2520_INPUT_H
#define DV2520_INPUT_H

enum InputKeyTypes {
    InputKeyTypes_NA,

    InputKeyTypes_W,
    InputKeyTypes_A,
    InputKeyTypes_S,
    InputKeyTypes_D,
    
    InputKeyTypes_UP,
    InputKeyTypes_LEFT,
    InputKeyTypes_DOWN,
    InputKeyTypes_RIGHT
};

class InputKey {
public:
    InputKey();
    InputKey( InputKeyTypes p_type );
    ~InputKey();

    bool isValid() const;

    InputKeyTypes getType() const;
protected:
private:
    InputKeyTypes m_type;
};

class InputMouse {
public:
    InputMouse();
    InputMouse( float p_dX, float p_dY );
    ~InputMouse();
    
    float getDX() const;
    float getDY() const;
protected:
private:
    float m_dX;
    float m_dY;
};

#endif // DV2520_INPUT_H