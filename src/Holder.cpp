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
 */
#include "Holder.h"
#include "WPILib.h"

const std::string Holder::GetName(const holder_t h)
{
	switch(h)
	{
	case TRANSITION:
		return "TRANSITION";
	case HOLDER_IN:
		return "HOLDER_IN";
	case HOLDER_OUT:
		return "HOLDER_OUT";
	case HOLDING:
		return "HOLDING";
	default:
		return "ERRRRRRRRRR";
	}
}

Holder::Holder(uint8_t ValveIn, uint8_t ValveOut, uint8_t safety)
{
	m_a = new Solenoid(ValveIn);
	m_b = new Solenoid(ValveOut);
	m_safety = new DigitalInput(safety);
	m_currentState = HOLDER_IN;
	m_targetState = HOLDER_IN;
	m_needFree = true;
	reset(0.0);
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
	reset(0.0);
	retract();
	return;
}

Holder::Holder(Solenoid *ValveIn, Solenoid *ValveOut, DigitalInput *safety)
{
	m_a = ValveIn; ///sets m_a to be the input valve
	m_b = ValveOut; ///sets m_b to be the output valve
	m_safety = safety; ///sets m_safety to be the safety
	m_currentState = HOLDER_IN;
	m_targetState = HOLDER_IN;
	m_needFree = false; ///sets it to not use more memory than has been allocated
	reset(0.0);
	retract();
	return;
}

void
Holder::setTargetPosition(holder_t p) /// either extends or retracts pistons based on value of p
{
	if(getCurrentPosition() == TRANSITION)
	{
		return;
	}

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
		case TRANSITION:
			break;
		default:
			break;
	}
	return;
}

Holder::holder_t
Holder::getCurrentPosition()   ///returns current state of pistons (in or out)
{
	if(!ready())
		m_currentState = TRANSITION;
	else if(getSensorState() == SWITCH_TRANSITION)
		m_currentState = TRANSITION;
	else if(getSensorState() == ON)
		m_currentState = HOLDING;
	else if (m_a->Get() == true)
		m_currentState = HOLDER_OUT;
	else
		m_currentState = HOLDER_IN;

	return m_currentState;
}

Holder::holder_t
Holder::getTargetPosition() ///says what the target state is
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
		if(m_sensorState == ON)
		{
			reset(SENSOR_SWITCH_TIME);
			m_sensorState = SWITCH_TRANSITION;
		}
		if(ready())
			m_sensorState = OFF;
	}

	return m_sensorState;

}

void
Holder::extend()
{
	if(m_targetState != HOLDER_OUT)
	{
		reset(PISTON_TRAVEL_TIME);
		m_targetState = HOLDER_OUT;
	}
	///close output, open input
	m_a->Set(true);
	m_b->Set(false);
	return;
}

void
Holder::retract()
{
	if (getCurrentPosition() == HOLDING || getCurrentPosition() == TRANSITION)   // must not retract with switch on
		return;

	if (m_targetState != HOLDER_IN) ///if the target state isn't holder in reset and make the target state holder in
	{
		reset(PISTON_TRAVEL_TIME);
		m_targetState = HOLDER_IN;
	}
	///close input, open output
	m_a->Set(false);
	m_b->Set(true);
	return;
}

void
Holder::reset(const long mils) ///doesn't report current position until timer clears
{
	timer = std::chrono::system_clock::now() + std::chrono::milliseconds(mils);
}

bool
Holder::ready()
{
	return std::chrono::system_clock::now() > timer;
}
