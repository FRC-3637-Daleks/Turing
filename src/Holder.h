/*
 * Holder.h
 *
 *  Created on: Feb 10, 2015
 *      Author: elija_000
 */

#ifndef _HOLDER_H_
#define _HOLDER_H_

#include <chrono>

#include "WPILib.h"
#include "Hardware.h"

#define PISTON_TRAVEL_TIME 200 // Milliseconds
#define SENSOR_SWITCH_TIME 500  // Milliseconds

class Solenoid;
class DigitalInput;

//using namespace std::chrono;

class Holder {
public:
	Holder(uint8_t ValveIn, uint8_t ValveOut, uint8_t safety);
	Holder(Solenoid &ValveIn, Solenoid &ValveOut, DigitalInput &safety);
	Holder(Solenoid *ValveIn, Solenoid *ValveOut, DigitalInput *safety);

	enum holder_t {TRANSITION=-1, HOLDER_IN=0, HOLDER_OUT, HOLDING, NUM_STATES };
	static const std::string GetName(const holder_t h);

	enum sensor_t {SWITCH_TRANSITION=-1, ON, OFF};

	void setTargetPosition(holder_t p);
	holder_t getCurrentPosition();
	holder_t getTargetPosition();
	void extend();
	void retract();
	sensor_t getSensorState();

private:
	void reset(const long mils);
	bool ready();

private:
		Solenoid *m_a;
		Solenoid *m_b;
		DigitalInput *m_safety;
		bool m_needFree;
		holder_t m_currentState;
		holder_t m_targetState;
		sensor_t m_sensorState;
		std::chrono::time_point<std::chrono::system_clock> timer;
};

#endif
