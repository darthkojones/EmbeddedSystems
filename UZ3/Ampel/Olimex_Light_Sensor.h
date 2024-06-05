#ifndef	_LIGHT_SENSOR_H
#define	_LIGHT_SENSOR_H

#include <Arduino.h>

class Olimex_Light_Sensor
{
public:
	Olimex_Light_Sensor (int _Pin);
	int Read ();
	float ReadPercentage ();
	
private:
	int Pin;
};

#endif
