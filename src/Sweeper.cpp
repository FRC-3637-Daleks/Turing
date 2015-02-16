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
	m_needFree = true;
	return;
}

Sweeper::Sweeper(CANTalon &m_talon)
{
	m_t = &m_talon;
	m_needFree = false;
	return;
}

Sweeper::Sweeper(CANTalon *m_talon)
{
	m_t = m_talon;
	m_needFree = false;
	return;
}


bool
goForward(float motorSpeed)
{
     m_t->Set(motorSpeed);
}

bool
goReverse(float motorSpeed)
{
     m_t->Set(motorSpeed);
}

bool
Stop()
{
     talonMotor->Set(0);
}
