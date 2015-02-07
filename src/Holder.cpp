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
#include "WPILib.h"

Holder::Holder(uint8_t ValveIn, uint8_t ValveOut)
{
	m_a = new Solenoid(ValveIn);
	m_b = new Solenoid(ValveOut);
	m_currentState = HOLDER_IN;
	m_holderState = HOLDER_IN;
	m_needFree = true;
	return;
};

Holder::Holder(Solenoid &ValveIn, Solenoid &ValveOut)
{
	m_a = &ValveIn;
	m_b = &ValveOut;
	m_currentState = HOLDER_IN;
	m_holderState = HOLDER_IN;
	m_needFree = false;
	return;
};

Holder::Holder(Solenoid *ValveIn, Solenoid *ValveOut)
{
	m_a = ValveIn;
	m_b = ValveOut;
	m_currentState = HOLDER_IN;
	m_holderState = HOLDER_IN;
	m_needFree = false;
	return;
}

void
Holder::setPosition(holder_t p) // either extends or retracts pistons based on value of p
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
	return;
}

holder_t
Holder::getPosition()   //returns current state of pistons (in or out)
{

	if (m_a->Get())
		m_holderState = HOLDER_IN;
	else
		m_holderState = HOLDER_OUT;
	//m_holderState = (m_a->Get() ? HOLDER_IN : HOLDER_OUT);
	return m_holderState;
}

void
extend()
{
	//close output, open input
	m_a->Set(true);
	m_b->Set(false);
	return;
}

void
retract()
{
	//close input, open output
	m_a->Set(false);
	m_b->Set(true);
	return;
}
