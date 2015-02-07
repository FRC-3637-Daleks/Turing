/*
 * Holder.h
 *
 *  Created on: Jan 31, 2015
 *      Author: elija_000
 */
#ifndef _HOLDER_H_
#define _HOLDER_H_

#include "WPILib.h"
#include "Hardware.h"

class Solenoid;

class Holder {
public:
	Holder(uint8_t ValveIn, uint8_t ValveOut);
	Holder(Solenoid &ValveIn, Solenoid &ValveOut);
	Holder(Solenoid *ValveIn, Solenoid *ValveOut);

	void setPosition(holder_t p);
	holder_t getPosition();
	void extend();
	void retract();

private:
		Solenoid *m_a;
		Solenoid *m_b;
		bool free_needed;
		char m_currentState;
		char m_holderState;
		enum holder_t { HOLDER_IN, HOLDER_OUT, NUM_STATES };
};

#endif





