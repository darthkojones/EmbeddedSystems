#ifndef	_JOYSTICK_H
#define	_JOYSTICK_H

#include <Arduino.h>

class Olimex_Joystick
{
public:
	Olimex_Joystick (int PinX, int PinY);
	Olimex_Joystick (int PinX, int PinY, int PinBut);
	int X_Raw ();
	int Y_Raw ();
	float X ();
	float Y ();
	bool But ();
private:
	int X_Axis, Y_Axis, B;
	bool Button_on_digital_pin;
};

#endif
