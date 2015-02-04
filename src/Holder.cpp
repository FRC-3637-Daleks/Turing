/*
* Holder.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: elija_000

XX Holder() - constructs Holder

XX enum Position_t PISTON_OUT, PISTON_IN

XX Holder_t getPosition() - returns current state of pistons (in or out)

XX setPosition(holder_t p) - either extends or retracts pistons based on value of p

XX retract() - retracts pistons

XX extend() - extends pistons

 */
#include "Holder.h"


void Holder::setPosition(holder_t p) // either extends or retracts pistons based on value of p
{
	m_currentState = p;
	switch(m_currentState) {
		case HOLDER_IN:
			retract();
			break;
		case HOLDER_OUT:
			extend();
			break;
		default:
			break;
		}
}

void Holder::getPosition()   //returns current state of pistons (in or out)
{
	m_currentState = /*  NOT SURE HOW TO DERIVE CURRENT STATS*/;
	return m_currentState;
}

void extend()
{
	/*  NOT SURE YET */
}

void retract()
{
	/*  NOT SURE YET */
}

}


