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


void
Sweeper::goForward(float motorSpeed)
{

	m_t->Set(motorSpeed);
return;}

void
Sweeper::goReverse(float motorSpeed)
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
