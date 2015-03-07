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

class Solenoid; ///invokes the class Solenoid so we can use it
class DigitalInput;

class Holder {  ///gives a model for class Holder
public:
	Holder(uint8_t ValveIn, uint8_t ValveOut, uint8_t safety);
	Holder(Solenoid &ValveIn, Solenoid &ValveOut, DigitalInput &safety); ///3 different inputs just in case we need it
	Holder(Solenoid *ValveIn, Solenoid *ValveOut, DigitalInput *safety);

	enum holder_t {TRANSITION=-1, HOLDER_IN=0, HOLDER_OUT, HOLDING, NUM_STATES };	///different states of the holder mechanism
	static const std::string GetName(const holder_t h);

	enum sensor_t {SWITCH_TRANSITION=-1, ON, OFF};

	void setTargetPosition(holder_t p); ///tells the holder to go to set position
	holder_t getCurrentPosition(); ///says what the current position
	holder_t getTargetPosition(); ///says what position you want to go to
	void extend(); ///tells the pistons to go out
	void retract(); ///tells the pistons to go in
	sensor_t getSensorState();

private:
	void reset(const long mils);
	bool ready();

private:
		Solenoid *m_a; ///links the pointer m_a to class Solenoid
		Solenoid *m_b; ///links the pointer m_b to class Solenoid
		DigitalInput *m_safety; ///m_safety checks to see if the pistons are out, if
		///they are the arms cannot retract unless the safety is off
		bool m_needFree;
		holder_t m_currentState; ///says what the state of the holder is
		holder_t m_targetState; ///says what the state of the holder should be
		sensor_t m_sensorState;
		std::chrono::time_point<std::chrono::system_clock> timer;
};

#endif
