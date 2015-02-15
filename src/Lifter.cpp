#include "Lifter.h"
#include "stdio.h"
#include "WPILib.h"

// States are in Inches
const double Lifter::States[] = {
		[Lifter::Ground]=3.0,
		[Lifter::Step]=8.5,
		[Lifter::StackUp]=12.0,
		[Lifter::Holding]=20.0,
		[Lifter::ToteUp]=24.5,
		[Lifter::BinT1]=39.0,
		[Lifter::BinT2]=51.0,
		[Lifter::BinT3]=63.0,
		[Lifter::Top]=75.0
};

Lifter::Lifter(int talID1, int talID2, PIDConfig iPID, double ramp): m_tal1(talID1), m_tal2(talID2), pid(iPID), rampRate(ramp)
{
	targetPosition = 0.0;
	targetState = Height_t::Ground;

	m_tal2.SetControlMode(CANSpeedController::kFollower);
	m_tal2.Set(talID1);

	calibrate();
}

void Lifter::calibrate()
{
	if(!m_tal1.GetReverseLimitOK())
	{
		m_tal1.SetFeedbackDevice(CANTalon::QuadEncoder);
		m_tal1.SetPID(pid.P, pid.I, pid.D);
		m_tal1.SetIzone(pid.iZone);
		m_tal1.SetCloseLoopRampRate(rampRate);
		m_tal1.ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
		m_tal1.SetSensorDirection(true);	// Reverse sensors
		m_tal1.SetControlMode(CANTalon::ControlMode::kPosition);
		m_tal1.SetPosition(0.0);
		targetPosition = 0.0;
		calibrated = true;
	}
	else
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
		m_tal1.SetVoltageRampRate(rampRate);
		m_tal1.Set(-0.5);
		calibrated = false;
	}
}

bool Lifter::setTargetPosition(double position)
{
	if(!isCalibrated())
	{
		calibrate();
		return false;
	}

	targetPosition = position;

	if (targetPosition < m_tal1.GetPosition() && !m_tal1.GetReverseLimitOK())
	{
		targetPosition = m_tal1.GetPosition();
	}
	else if (targetPosition > m_tal1.GetPosition() && !m_tal1.GetForwardLimitOK())
	{
		targetPosition = m_tal1.GetPosition();
	}

	m_tal1.Set(targetPosition);
	if (getCurrentPosition() == targetPosition)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Lifter::getTargetPosition()
{
	return targetPosition;
}

double Lifter::getCurrentPosition()
{
	return m_tal1.GetPosition();
}

double Lifter::getDistanceToTarget()
{
	return (Lifter::getTargetPosition() - Lifter::getCurrentPosition());
}

bool Lifter::setTargetState(Height_t h)
{
	targetState = h;

	setTargetPosition(inchesOffGroundToTicks(States[targetState]));

	if (Lifter::getCurrentState() == Lifter::getTargetState())
	{
		if(Lifter::getCurrentPosition() <= 100 && targetState == Ground)
		{
			calibrate();
			return isCalibrated();
		}
		return true;
	}
	else
	{
		return false;
	}
}

Lifter::Height_t Lifter::getTargetState()
{
	return targetState;
}

Lifter::Height_t Lifter::getCurrentState()
{
	for(int i = Ground; i <= Top; i++)
	{
		if(fabs(getCurrentPosition() - inchesOffGroundToTicks(States[i])) < toleranceTicks) 	// Checks to see if its within the tolerance of the state
		{
			return Height_t(i);
		}
	}
	return Height_t::Ground;
}

void Lifter::offsetTarget(double offset)	// Inches
{
	setTargetPosition(targetPosition + inchToTicks(offset));
}

double Lifter::inchToTicks(double inches)
{
	return (inches * ticksPerInch);
}

double Lifter::inchesOffGroundToTicks(double inches)
{
	return inchToTicks((inches - inchesOffGround)); // Subtracting distance to ground
}
