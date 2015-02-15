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

class CANTalon;
class CANTalon;
class DigitalInput;
class Joystick;


class Sweeper {

	Sweeper(uint8_t m_talon);
	Sweeper(CANTalon &m_talon);
	Sweeper(CANTalon *m_talon);

	enum sweeper_t { SWEEPER_UP, SWEEPER_DOWN, NUM_STATES };


	sweeper_t setPosition(sweeper_t p);
	sweeper_t getPosition();
	void lowerSweeper();
	void raiseSweeper();

private:
		CANTalon m_motor;
	    Joystick m_stick;
		bool m_needFree;
		DigitalInput *m_safety;

		sweeper_t m_currentState;
		sweeper_t m_targetState;

};

#endif
