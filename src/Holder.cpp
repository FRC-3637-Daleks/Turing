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

<<<<<<< HEAD
XX extend() - extends pistons
=======
XX extend() - extefnds pistons
>>>>>>> origin/Dev-elijah-4real-MKII

 */
#include "Holder.h"
#include "WPILib.h"

Holder::Holder(uint8_t ValveIn, uint8_t ValveOut, uint8_t safety)
{
	m_a = new Solenoid(ValveIn);
	m_b = new Solenoid(ValveOut);
	m_safety = new DigitalInput(safety);
	m_currentState = HOLDER_IN;
	m_targetState = HOLDER_IN;
	m_needFree = true;
	retract();
	return;
}

Holder::Holder(Solenoid &ValveIn, Solenoid &ValveOut, DigitalInput &safety)
{
	m_a = &ValveIn;
	m_b = &ValveOut;
	m_safety = &safety;
	m_currentState = HOLDER_IN;
	m_targetState = HOLDER_IN;
	m_needFree = false;
	retract();
	return;
}

Holder::Holder(Solenoid *ValveIn, Solenoid *ValveOut, DigitalInput *safety)
{
	m_a = ValveIn;
	m_b = ValveOut;
	m_safety = safety;
	m_currentState = HOLDER_IN;
	m_targetState = HOLDER_IN;
	m_needFree = false;
	retract();
	return;
}

void
Holder::setTargetPosition(holder_t p) // either extends or retracts pistons based on value of p
{
	switch(p) {
		case HOLDER_IN:
			retract();
			break;
		case HOLDER_OUT:
			extend();
			break;
		case HOLDING:
			extend();
			break;
		default:
			break;
	}
	return;
}

Holder::holder_t
Holder::getCurrentPosition()   //returns current state of pistons (in or out)
{
	if (m_a->Get() == false && waitExceeded())
		m_currentState = HOLDER_IN;
	else if(getSensorState() == ON)
		m_currentState = HOLDING;
	else if (waitExceeded())
		m_currentState = HOLDER_OUT;
	// else it remains the same

	return m_currentState;
}

Holder::holder_t
Holder::getTargetPosition()
{
	return m_targetState;
}

Holder::sensor_t
Holder::getSensorState()
{
	if (!m_safety->Get())
	{
		m_sensorState = ON;
	}
	else
	{
		m_sensorState = OFF;
	}

	return m_sensorState;

}

void
Holder::extend()
{
	if(m_targetState != HOLDER_OUT)
	{
		reset();
		m_targetState = HOLDER_OUT;
	}
	//close output, open input
	m_a->Set(true);
	m_b->Set(false);
	return;
}

void
Holder::retract()
{
	if (getCurrentPosition() == HOLDING)   // must not retract with switch on
		return;

	if (m_targetState != HOLDER_IN)
	{
		reset();
		m_targetState = HOLDER_IN;
	}
	//close input, open output
	m_a->Set(false);
	m_b->Set(true);
	return;
}

void
Holder::reset()
{
	timer = clock();
}

bool
Holder::waitExceeded(double mils)
{
	return clock() - timer >  mils*CLOCKS_PER_SEC/1000.0;
}
