/*
 * Holder.h
 *
 *  Created on: Feb 10, 2015
 *      Author: elija_000
 */

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

	enum holder_t { HOLDER_IN, HOLDER_OUT, NUM_STATES };

enum sensor_t {ON, OFF};

	void setPosition(holder_t p);
	holder_t getPosition();
	void extend();
	void retract();
	sensor_t getSensorState(sensor_t Safety);

private:
		Solenoid *m_a;
		Solenoid *m_b;
		bool m_needFree;
		holder_t m_currentState;
		holder_t m_holderState;
		sensor_t Safety;

};

#endif
