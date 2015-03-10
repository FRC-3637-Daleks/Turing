/*
 * Sweeper.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: elija_000
 */

#include "Sweeper.h"
#include "WPILib.h"

// States are in Inches
const double Sweeper::States[] = {
		[Sweeper::Down]=0.0,
		[Sweeper::Intermediate]=5.0,
		[Sweeper::Up]=10.0
};


Sweeper::Sweeper(uint32_t talID1, Lifter::PIDConfig iPID, double ramp): m_tal1(talID1), pid(iPID), rampRate(ramp), targetState(Sweeper::Up)
{
	m_tal1.SetControlMode(CANTalon::ControlMode::kPosition);
	m_tal1.SetFeedbackDevice(CANTalon::AnalogPot);
	m_tal1.SetPID(iPID.P, iPID.I, iPID.D);
	m_tal1.SetIzone(iPID.iZone);
	m_tal1.SetCloseLoopRampRate(ramp);
	m_tal1.ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
	m_tal1.SetSensorDirection(true);
	setState(targetState);
	return;
}

void
Sweeper::setState(State_t state)
{
	targetState = state;
	setPosition(States[state]);
	return;
}

void
Sweeper::setPosition(double pos)
{
	targetPosition = pos;
	m_tal1.Set(pos);
	return;
}


void
Sweeper::offset(double off)
{
	setPosition(getCurrentPosition()+off);
	return;
}

void
Sweeper::Stop()
{
	m_tal1.StopMotor();
	return;
}

double
Sweeper::getCurrentPosition()
{
	return m_tal1.GetPosition();
}

double
Sweeper::getTargetPosition()
{
	return targetPosition;
}

Sweeper::State_t
Sweeper::getTargetState()
{
	return targetState;
}
