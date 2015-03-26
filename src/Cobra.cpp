/*
 * Cobra.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: elija_000
 */

#include "Cobra.h"
#include "WPILib.h"

// States are in Inches
const double Cobra::States[] = {
		[Cobra::Down]=430,
		[Cobra::Intermediate]=700,
		[Cobra::Up]=850.0
};

std::string Cobra::GetStateName(const State_t state)
{
	switch(state)
	{
	case Cobra::Down:
		return "Down";
	case Cobra::Intermediate:
		return "Intermediate";
	case Cobra::Up:
		return "Up";
	default:
		return "Err";
	}
}

Cobra::Cobra(uint32_t talID1, Lifter::PIDConfig iposPID, Lifter::PIDConfig ivelPID, double ramp): LogObject<Cobra>(this), m_tal1(talID1), positionPID(iposPID), velocityPID(ivelPID) ,rampRate(ramp), targetState(Cobra::Up), mode(Velocity)
{
	LogText()<<"Constructor started for (talID1: "<<talID1<<
			", iposPID: {"<<iposPID.P<<", "<<iposPID.I<<", "<<iposPID.D<<
			"}, ivelPID: {"<<ivelPID.P<<", "<<ivelPID.I<<", "<<ivelPID.D<<
			"}, ramp: "<<ramp;
	holdPosition = -1.0;
	setMode(Position);	// Do Not Remove!
	m_tal1.SetControlMode(CANTalon::ControlMode::kPosition);
	m_tal1.SetFeedbackDevice(CANTalon::AnalogPot);
	m_tal1.SelectProfileSlot(0);
	m_tal1.SetCloseLoopRampRate(rampRate);
	m_tal1.SetVoltageRampRate(rampRate);
	m_tal1.ConfigLimitMode(CANTalon::LimitMode::kLimitMode_SoftPositionLimits);
	m_tal1.ConfigReverseLimit(States[Cobra::Down]);
	m_tal1.ConfigForwardLimit(States[Cobra::Up]);
	//m_tal1.SetSensorDirection(false);
	setState(targetState);
	AddLog<std::string>("target_state", [this]() {return GetStateName(State_t(getTargetState()));}, 0);
	AddLog<std::string>("current_state", [this]() {return GetStateName(State_t(getTargetState()));}, 0);
	AddLog<double>("target_position", &Cobra::getTargetPosition, 0);
	AddLog<double>("current_position", &Cobra::getCurrentPosition, 0);
	AddLog<double>("hold_position", [this]() {return holdPosition;}, 0);
	setMode(Position);
	LogText()<<"Constructor Complete";
	return;
}

void Cobra::setMode(Mode_t m)
{
	if(mode == m)
		return;
	mode = m;

	if(mode == Position)
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPosition);
		LogText()<<"Switching to Positional mode";
	}
	else if (mode == Velocity)
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kSpeed);
		LogText()<<"Switching to Velocity mode";
	}
	else
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
		LogText(DRR::LEVEL_t::ALERT)<<"Switching to Raw Voltage mode";
		return;
	}

	Lifter::PIDConfig pid(mode == Position? positionPID:velocityPID);
	m_tal1.SetPID(pid.P, pid.I, pid.D);
}

void
Cobra::setState(State_t state)
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
Cobra::setPosition(double pos)
{
	setMode(Position);
	targetPosition = pos;
	m_tal1.Set(targetPosition);
	return;
}


void
Cobra::offset(double off)
{
	targetState = Transition;
	setPosition(getCurrentPosition()+off);
	return;
}

void
Cobra::setVelocity(double vel)
{
	targetState = Transition;
	setMode(Velocity);
	m_tal1.Set(50.0*vel);
	return;
}

double
Cobra::getVelocity()
{
	return m_tal1.GetSpeed();
}

void
Cobra::setVBus(double vel)
{
	targetState = Transition;
	setMode(RawVoltage);
	m_tal1.Set(vel);
	return;
}

void
Cobra::stop()
{
	setState(Hold);
	return;
}

double
Cobra::getCurrentPosition()
{
	return m_tal1.GetPosition();
}

double
Cobra::getTargetPosition()
{
	return targetPosition;
}

int
Cobra::getTargetState()
{
	return targetState;
}

int
Cobra::getCurrentState()
{
	if(targetState == Hold)
		return Hold;
	auto pos = getCurrentPosition();
	State_t state = Transition;
	for(int i = 0; i < NUM_STATES; i++)
	{
		if(fabs(States[i]-pos) < thresh)
		{
			state = State_t(i);
			break;
		}
	}

	return state;
}
