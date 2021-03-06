#include "Lifter.h"
#include "stdio.h"
#include "WPILib.h"
#include "Config.h"

// States are in Inches
const double Lifter::States[] = {
		[Lifter::Ground]=3.0,
		[Lifter::Step]=8.5,
		[Lifter::StackDown]=9.0,
		[Lifter::StackUp]=13.0,
		[Lifter::ToteScore]=14.5,
		[Lifter::ToteDown]=20.0,
		[Lifter::Chute]=22.0,
		[Lifter::ToteUp]=29.0,
		[Lifter::BinT1]=39.0,
		[Lifter::BinT2]=51.0,
		[Lifter::BinT3]=63.0,
		[Lifter::Top]=75.0
};

double Lifter::ticksPerInch = 120.0;

const std::string Lifter::GetName(const Height_t h)
{
	switch(h)
	{
	case Lifter::TRANSITION:
		return "TRANSITION;";
	case Lifter::Ground:
		return "Ground";
	case Lifter::Step:
		return "Step";
	case Lifter::StackDown:
		return "StackDown";
	case Lifter::StackUp:
		return "StackUp";
	case Lifter::ToteScore:
		return "ToteScore";
	case Lifter::ToteDown:
		return "ToteDown";
	case Lifter::Chute:
		return "Chute";
	case Lifter::ToteUp:
		return "ToteUp";
	case Lifter::BinT1:
		return "BinT1";
	case Lifter::BinT2:
		return "BinT2";
	case Lifter::BinT3:
		return "BinT3";
	case Lifter::Top:
		return "Top";
	default:
		return "ERRRRRRR";
	}
}

Lifter::Lifter(int talID1, int talID2, PIDConfig iPID, double ramp): LogObject<Lifter>(this), m_tal1(talID1), m_tal2(talID2), pid(iPID), rampRate(ramp)
{
	LogText()<<"Constructed with (talID1: "<<talID1<<", talID2: "<<talID2<<
			", PID: ("<<pid.P<<", "<<pid.I<<", "<<pid.D<<"), ramp: "<<ramp;
	targetPosition = 0.0;
	targetState = Height_t::Ground;

	m_tal2.SetControlMode(CANSpeedController::kFollower);
	m_tal2.Set(talID1);

	LogText()<<"Configuring Talon "<<talID2<<" to follow Talon"<<talID1;

	AddLog("target_position", &Lifter::getTargetPosition, 0);
	AddLog("current_position", &Lifter::getCurrentPosition, 0);
	AddLog<std::string>("target_state", [this]() -> std::string {return Lifter::GetName(getTargetState());}, 0);
	AddLog<std::string>("current_state", [this]() -> std::string {return Lifter::GetName(getCurrentState());}, 0);

	AddLog<double>("talons/master/voltage", std::bind(&CANTalon::GetOutputVoltage, &m_tal1), 0);
	AddLog<double>("talons/master/current", std::bind(&CANTalon::GetOutputCurrent, &m_tal1), 0);
	AddLog<double>("talons/slave/voltage", std::bind(&CANTalon::GetOutputVoltage, &m_tal2), 0);
	AddLog<double>("talons/slave/current", std::bind(&CANTalon::GetOutputCurrent, &m_tal2), 0);

	calibrate();
	LogText()<<"Constructor Complete";
}

void Lifter::calibrate()
{
	// If the Limit Switch is triggered
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
		targetState = Ground;
		if(calibrated == false)
			LogText()<<"Calibrated";
		calibrated = true;
	}
	// If the Limit Switch hasn't been triggered.
	else
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
		m_tal1.SetVoltageRampRate(rampRate);
		m_tal1.Set(-0.5);
		//targetState = TRANSITION;
		if(calibrated == true)
			LogText()<<"Calibrating";
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
	if(targetState != h && targetState != TRANSITION && h != TRANSITION)
		LogText()<<"Changing Lifter State from "<<GetName(targetState)<<" to "<<GetName(h);

	targetState = h;

	setTargetPosition(inchesOffGroundToTicks(States[targetState]));

	if(Lifter::getCurrentPosition() <= 200 && targetState == Ground)
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
			return Ground;

	for(int i = Ground; i < Top; i++)
	{
		if(fabs(getCurrentPosition() - inchesOffGroundToTicks(States[i])) < toleranceTicks) 	// Checks to see if its within the tolerance of the state
		{
			return Height_t(i);
		}
	}

	if(targetState == TRANSITION)
		return TRANSITION;


	return Top;
}

void Lifter::offsetTarget(double offset)	// Inches
{
	targetState = TRANSITION;
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
			m_tal1.Set(targetPosition - 100.0);
		}
		else if (getCurrentPosition() > getTargetPosition())
		{
			m_tal1.Set(targetPosition + 100.0);
		}
	}
}
