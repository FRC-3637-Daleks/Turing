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
#include "Lifter.h"

class Sweeper { //Sweeper no sweeping!
public:
	enum State_t {Down, Intermediate, Up, NUM_STATES};
	static const double States[NUM_STATES];

private:
	CANTalon m_tal1;
	Lifter::PIDConfig pid;
	double rampRate;
	double targetPosition;
	State_t targetState;

public:
	/// Constructor accepts talon ID, PID values, and a Ramp Rate.  and Sweeper is calibrated.

	Sweeper(uint32_t talID1, Lifter::PIDConfig iPID, double ramp);

public:
	void  setState(State_t state);
	void  setPosition(double pos);
	void  offset(double off);
	void  Stop();

public:
	double getCurrentPosition();
	double getTargetPosition();
	State_t getTargetState();
};

#endif
