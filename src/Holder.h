/*
 * Holder.h
 *
 *  Created on: Feb 10, 2015
 *      Author: elija_000
 */

#ifndef _HOLDER_H_
#define _HOLDER_H_

#include <ctime>

#include "WPILib.h"
#include "Hardware.h"

#define PISTON_TRAVEL_TIME 200 // Milliseconds

class Solenoid;
class DigitalInput;

class Holder {
public:
	Holder(uint8_t ValveIn, uint8_t ValveOut, uint8_t safety);
	Holder(Solenoid &ValveIn, Solenoid &ValveOut, DigitalInput &safety);
	Holder(Solenoid *ValveIn, Solenoid *ValveOut, DigitalInput *safety);

	enum holder_t { HOLDER_IN=0, HOLDER_OUT, HOLDING, NUM_STATES };

enum sensor_t {ON, OFF};

	void setTargetPosition(holder_t p);
	holder_t getCurrentPosition();
	holder_t getTargetPosition();
	void extend();
	void retract();
	sensor_t getSensorState();

private:
	void reset();
	bool waitExceeded(double mils=PISTON_TRAVEL_TIME);

private:
		Solenoid *m_a;
		Solenoid *m_b;
		DigitalInput *m_safety;
		bool m_needFree;
		holder_t m_currentState;
		holder_t m_targetState;
		sensor_t m_sensorState;
		clock_t timer;
};

#endif
