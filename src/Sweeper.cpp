/*
 * Sweeper.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: elija_000
 */

#include "Sweeper.h"
#include "WPILib.h"


Sweeper::Sweeper(uint32_t m_talon)
{
	m_t = new CANTalon(m_talon);
	m_needFree = false;
	m_t->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	return;
}

Sweeper::Sweeper(CANTalon &m_talon)
{
	m_t = &m_talon;
	m_needFree = false;
	m_t->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	return;
}

Sweeper::Sweeper(CANTalon *m_talon)
{
	m_t = m_talon;
	m_needFree = false;
	m_t->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	return;
}


void
Sweeper::setSpeed(float motorSpeed)
{
	m_t->Set(motorSpeed);
return;
}

void
Sweeper::Stop()
{
	m_t->Set(0);
return;
}
