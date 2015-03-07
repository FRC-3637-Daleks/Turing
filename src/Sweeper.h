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

class Sweeper { //Sweeper no sweeping!
public:

	Sweeper(uint32_t m_talon);
	Sweeper(CANTalon &m_talon);
	Sweeper(CANTalon *m_talon);

	void  setSpeed(float motorSpeed);
	void  Stop();

private:
		CANTalon *m_t;

		bool m_needFree;

};

#endif
