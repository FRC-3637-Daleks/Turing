/*
 * Sweeper.h
 *
 *  Created on: Feb 14, 2015
 *      Author: elija_000
 */

#ifndef SRC_SWEEPER_H_
#define SRC_SWEEPER_H_

#include "WPILib.h"
#include "Hardware.h"

class Talon;
class CANTalon;

class Sweeper {

	Sweeper(uint32_t m_talon);
	Sweeper(CANTalon &m_talon);
	Sweeper(CANTalon *m_talon);

	enum sweeper_t { SWEEPER_UP, SWEEPER_DOWN, NUM_STATES };


	bool  goForward(float motorSpeed);
	bool  goReverse(float motorSpeed);
	bool  Stop();

private:
		CANTalon *m_t;
		CANTalon m_motor;
		bool m_needFree;


};

#endif
