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


Sweeper::Sweeper(uint32_t m_talon, PIDConfig iPID, double ramp)
{
	m_t = new CANTalon(m_talon);
	m_needFree = true;
	m_t->SetControlMode(CANTalon::ControlMode::kPosition);
	m_t.SetFeedbackDevice(CANTalon::AnalogPot);
	m_t.SetPID(iPID.P, iPID.I, iPID.D);
	m_t.SetIzone(iPID.iZone);
	m_t.SetCloseLoopRampRate(ramp);
	m_t.ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
	m_t.SetSensorDirection(true);

	return;
}

Sweeper::Sweeper(CANTalon &m_talon, PIDConfig iPID, double ramp)
{
	m_t = &m_talon;
	m_needFree = false;
	m_t->SetControlMode(CANTalon::ControlMode::kPosition);
	m_t.SetFeedbackDevice(CANTalon::AnalogPot);
	m_t.SetPID(iPID.P, iPID.I, iPID.D);
	m_t.SetIzone(iPID.iZone);
	m_t.SetCloseLoopRampRate(ramp);
	m_t.ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
	m_t.SetSensorDirection(true);
	return;
}

Sweeper::Sweeper(CANTalon *m_talon, PIDConfig iPID, double ramp)
{
	m_t = m_talon;
	m_needFree = false;
	m_t->SetControlMode(CANTalon::ControlMode::kPosition);
	m_t.SetFeedbackDevice(CANTalon::AnalogPot);
	m_t.SetPID(iPID.P, iPID.I, iPID.D);
	m_t.SetIzone(iPID.iZone);
	m_t.SetCloseLoopRampRate(ramp);
	m_t.ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
	m_t.SetSensorDirection(true);
	return;
}

void
Sweeper::setState(Sweeper::States stateValue)
{
	m_t->setState(stateValue);
return;
}

void
Sweeper::setPosition(Sweeper_State_t newPos)
{
	m_t->Set(Sweeper::States[newPos]);
return;
}


void
Sweeper::setSpeed(float motorSpeed)
{
	m_t->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m_t->Set(motorSpeed);
	m_t->SetControlMode(CANTalon::ControlMode::kPosition);
return;
}

void
Sweeper::Stop()
{
	m_t->Set(0);
return;
}
