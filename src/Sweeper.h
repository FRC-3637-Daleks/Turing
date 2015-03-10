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

	Sweeper(uint32_t m_talon);
	Sweeper(CANTalon &m_talon);
	Sweeper(CANTalon *m_talon);

	struct PIDConfig
	{
		double P;
		double I;
		double D;
		unsigned int iZone;
		PIDConfig(const double iP, const double iI=0.0, const double iD=0.0, const unsigned int iiZone=200): P(iP), I(iI), D(iD), iZone(iiZone) {};
	};

	enum Sweeper_State_t {Down, Intermediate, Up, NUM_STATES};


private:
	CANTalon m_tal1;
	PIDConfig pid;
	double rampRate;
	double targetPosition;
	Height_t targetState;
	bool calibrated;

public:
	/// Constructor accepts talon ID, PID values, and a Ramp Rate.  and Sweeper is calibrated.

	Sweeper(int talID1, PIDConfig iPID, double ramp);
	Sweeper(CANTalon *m_talon, PIDConfig iPID, double ramp);
	Sweeper(CANTalon &m_talon, PIDConfig iPID, double ramp);

	void  setSpeed(float motorSpeed);
	void  Stop();

private:
		CANTalon *m_t;

		bool m_needFree;

};

#endif
