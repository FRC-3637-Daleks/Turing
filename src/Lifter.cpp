#include "Lifter.h"
#include "stdio.h"
#include "WPILib.h"
#include "Config.h"

// States are in Inches
const double Lifter::States[] = {
		[Lifter::Ground]=3.0,
		[Lifter::Step]=8.5,
		[Lifter::StackDown]=9.0,
		[Lifter::StackUp]=12.0,
		[Lifter::ToteDown]=20.0,
		[Lifter::ToteUp]=25.0,
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
		std::cout<<"Calibrated"<<std::endl;
	}
	else
	{
		std::cout<<"Calibrating"<<std::endl;
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
		calibrate();
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


	if(Lifter::getCurrentPosition() <= 100 && targetState == Ground)
	{
		calibrate();
		return isCalibrated();
	}

	if (Lifter::getCurrentState() == Lifter::getTargetState())
	{
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
	if(!m_tal1.GetReverseLimitOK())
		return previousState = Ground;

	for(int i = Ground; i < Top; i++)
	{
		if(fabs(getCurrentPosition() - inchesOffGroundToTicks(States[i])) < toleranceTicks) 	// Checks to see if its within the tolerance of the state
		{
			return previousState = Height_t(i);
		}
		else if(getCurrentPosition() > inchesOffGroundToTicks(States[i]) && getCurrentPosition() < inchesOffGroundToTicks(States[i+1]))
		{
			if(previousState > i+1)
				return previousState = Height_t(i+1);
			else if(previousState < i)
				return previousState = Height_t(i);
		}
	}
	return previousState;
}

void Lifter::offsetTarget(double offset)	// Inches
{
	setTargetPosition(getCurrentPosition() + inchToTicks(offset));
}

double Lifter::inchToTicks(double inches)
{
	return (inches * ticksPerInch);
}

double Lifter::inchesOffGroundToTicks(double inches)
{
	return inchToTicks((inches - inchesOffGround)); // Subtracting distance to ground
}

void Lifter::check()
{
	if (m_tal1.GetOutputCurrent() > MAX_TALON_CURRENT)
	{
		if (getCurrentPosition() < getTargetPosition())
		{
			setTargetPosition(targetPosition - 100.0);
		}
		else if (getCurrentPosition() > getTargetPosition())
		{
			setTargetPosition(targetPosition + 100.0);
		}
	}
}
