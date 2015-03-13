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
		[Sweeper::Down]=450,
		[Sweeper::Intermediate]=700,
		[Sweeper::Up]=900.0
};


Sweeper::Sweeper(uint32_t talID1, Lifter::PIDConfig iposPID, Lifter::PIDConfig ivelPID, double ramp): LogObject<Sweeper>(this), m_tal1(talID1), positionPID(iposPID), velocityPID(ivelPID) ,rampRate(ramp), targetState(Sweeper::Up)
{
	LogText()<<"Constructor started for (talID1: "<<talID1<<
			", iposPID: {"<<iposPID.P<<", "<<iposPID.I<<", "<<iposPID.D<<
			"}, ivelPID: {"<<ivelPID.P<<", "<<ivelPID.I<<", "<<ivelPID.D<<
			"}, ramp: "<<ramp;
	holdPosition = -1.0;
	setMode(Position);
	m_tal1.SetFeedbackDevice(CANTalon::AnalogPot);
	m_tal1.SelectProfileSlot(0);
	m_tal1.SetCloseLoopRampRate(ramp);
	m_tal1.SetVoltageRampRate(ramp);
	m_tal1.ConfigLimitMode(CANTalon::LimitMode::kLimitMode_SwitchInputsOnly);
	m_tal1.ConfigReverseLimit(States[Sweeper::Up]);
	m_tal1.ConfigForwardLimit(States[Sweeper::Down]);
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
	m_tal1.SetPID(pid.P, pid.I, pid.D);
}

void
Sweeper::setState(State_t state)
{
	if(state == Hold && targetState != Hold)
	{
		holdPosition = getCurrentPosition();
	}
	else if(state != Hold)
	{
		holdPosition = -1.0;
	}

	targetState = state;
	if(targetState == Transition)
		return;
	else if(targetState == Hold)
		setPosition(holdPosition);
	else
		setPosition(States[targetState]);
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
	targetState = Transition;
	setPosition(getCurrentPosition()+off);
	return;
}

void
Sweeper::setVelocity(double vel)
{
	targetState = Transition;
	setMode(Velocity);
	m_tal1.Set(50.0*vel);
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
	targetState = Transition;
	setMode(RawVoltage);
	m_tal1.Set(vel);
	return;
}

void
Sweeper::stop()
{
	setState(Hold);
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
