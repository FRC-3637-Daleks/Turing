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
#include "LogObject.h"

class Sweeper: public DRR::LogObject<Sweeper> { //Sweeper no sweeping!
public:
	enum State_t {Hold=-2, Transition=-1, Down=0, Intermediate, Up, NUM_STATES};
	enum Mode_t {Velocity=0, Position, RawVoltage};
	static const double States[NUM_STATES];
	static constexpr double thresh = 3.0;

private:
	CANTalon m_tal1;
	Lifter::PIDConfig positionPID, velocityPID;
	double rampRate;
	double targetPosition;
	double holdPosition;
	State_t targetState;
	Mode_t mode;

public:
	/// Constructor accepts talon ID, PID values, and a Ramp Rate.  and Sweeper is calibrated.

	Sweeper(uint32_t talID1, Lifter::PIDConfig iposPID, Lifter::PIDConfig ivelPID, double ramp);

public:
	void  setMode(Mode_t mode);
	bool setPositionMode() {return mode;};

public:	/// Positional functions
	void  setState(State_t state);
	void  setPosition(double pos);
	void  offset(double off);
	void  stop();

public:	/// Velocity functions
	void  setVelocity(double vel);
	double  getVelocity();

public: /// vBus functions
	void setVBus(double vel);

public:
	double getHoldPosition();
	double getCurrentPosition();
	double getTargetPosition();
	int getTargetState();
	int getCurrentState();
};

#endif
