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
	InputMouse( double p_dX, double p_dY );
	~InputMouse();
	
	double getDX() const;
	double getDY() const;
protected:
private:
	double m_dX;
	double m_dY;
};

#endif // DV2520_INPUT_H