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
		[Sweeper::Down]=500,
		[Sweeper::Intermediate]=700,
		[Sweeper::Up]=900.0
};


Sweeper::Sweeper(uint32_t talID1, Lifter::PIDConfig iposPID, Lifter::PIDConfig ivelPID, double ramp): m_tal1(talID1), positionPID(iposPID), velocityPID(ivelPID) ,rampRate(ramp), targetState(Sweeper::Up)
{
	LogText()<<"Constructor started for (talID1: "<<talID1<<
			", iposPID: {"<<iposPID.P<<", "<<iposPID.I<<", "<<iposPID.D<<
			"}, ivelPID: {"<<ivelPID.P<<", "<<ivelPID.I<<", "<<ivelPID.D<<
			"}, ramp: "<<ramp;
	setMode(Position);
	m_tal1.SetFeedbackDevice(CANTalon::AnalogPot);
	m_tal1.SetCloseLoopRampRate(ramp);
	m_tal1.ConfigLimitMode(CANTalon::LimitMode::kLimitMode_SwitchInputsOnly);
	//m_tal1.ConfigReverseLimit(States[Sweeper::Up]);
	//m_tal1.ConfigForwardLimit(States[Sweeper::Down]);
	//m_tal1.SetSensorDirection(false);
	setState(targetState);
	LogText()<<"Constructor Complete";
	return;
}

void Sweeper::setMode(Mode_t m)
{
	if(mode == m)
		return;
	mode = m;

	if(mode == Position)
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPosition);
	}
	else if (mode == Velocity)
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kSpeed);
	}
	else
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
		return;
	}

	Lifter::PIDConfig pid(mode == Position? positionPID:velocityPID);
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
	setMode(Position);
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
Sweeper::setVelocity(double vel)
{
	setMode(Velocity);
	m_tal1.Set(vel);
	return;
}

double
Sweeper::getVelocity()
{
	return m_tal1.GetSpeed();
}

void
Sweeper::setVBus(double vel)
{
	setMode(RawVoltage);
	m_tal1.Set(vel);
	return;
}

void
Sweeper::stop()
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
