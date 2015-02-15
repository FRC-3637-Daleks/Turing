/*
 * Sweeper.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: elija_000
 */

#include "Sweeper.h"
#include "WPILib.h"


Sweeper::Sweeper(uint8_t m_talon)
{
	m_t = new CANTalon(m_talon);
	m_currentState = SWEEPER_UP;
	m_targetState = SWEEPER_UP;
	m_needFree = true;
	return;
}

Sweeper::Sweeper(CANTalon &m_talon)
{
	m_t = &m_talon;
	m_currentState = SWEEPER_UP;
	m_targetState = SWEEPER_UP;
	m_needFree = false;
	return;
}

Sweeper::Sweeper(CANTalon *m_talon)
{
	m_t = m_talon;
	m_currentState = SWEEPER_UP;
	m_targetState = SWEEPER_UP;
	m_needFree = false;
	return;
}

sweeper_t
Sweeper::setPosition(sweeper_t p) // either RAISES or LOWERS Sweeper based on Input value
{
	m_targetState = p;
	switch(m_targetState) {
		case SWEEPER_UP:
			raiseSweeper();
			break;
		case SWEEPER_DOWN:
			lowerSweeper();
			break;
		default:
			break;
	}
	return;
}

sweeper_t
Sweeper::getPosition()   //returns current state of Sweeper (SWEEPER_UP or SWEEPER_DOWN)
{
	if (m_t->Get()=0)  //If GET from TALON object returns 0, then it is UP
		m_sweeperState = SWEEPER_UP;
	else
		m_sweeperState = SWEEPER_DOWN;

	return m_sweeperState;
}

void
Sweeper::lowerSweeper()
{
	//Figure out the current and Target Sweeper positions, see if movement is needed
	m_currentState = getPosition();
	m_targetState =  setPosition();

	//If currentPosition != targetPosition then move
	if (m_currentState != m_targetState)
			setPosition (m_targetState);
	return;
}

void
Sweeper::raiseSweeper()
{
//Figure out the current and Target Sweeper positions, see if movement is needed
	m_currentState = Sweeper::getPosition();
	m_targetState = Sweeper::setPosition();

	//If currentPosition != targetPosition then move
	if (m_currentState != m_targetState)
			setPosition (m_targetState);
	return;
}


