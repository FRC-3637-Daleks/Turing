#include "Lifter.h"
#include "stdio.h"
#include "WPILib.h"

// States are in Inches
//	enum Height_t {Ground = 0, Step, StackUp, Holding, ToteUp, BinT1, BinT2, BinT3, Top};
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

Lifter::Lifter(int talID1, int talID2,
		double P, double I, double D, int iZone, double rampRate,
		double upLim, double lowLim,
		double ticksPerInch, double inchesOffGround): m_tal1(talID1), m_tal2(talID2)
{
	m_P = P;
	m_I = I;
	m_D = D;
	m_ticksPerInch = ticksPerInch;	// Temporarily 600
	m_upLim = (upLim * ticksPerInch);	// Input in inches
	m_lowLim = (lowLim * ticksPerInch);	// Input in inches
	m_targetPos = 0.0;
	m_targetState = Height_t::Ground;
	m_iZone = iZone;
	m_rampRate = rampRate;
	m_inchesOffGround = inchesOffGround;

	calibrate();

	m_tal2.SetControlMode(CANSpeedController::kFollower);
	m_tal2.Set(talID1);

}

void Lifter::calibrate()
{
	if(!m_tal1.GetReverseLimitOK())
	{
		//m_tal1.SelectProfileSlot(0);
		m_tal1.SetFeedbackDevice(CANTalon::QuadEncoder);
		m_tal1.SetPID(m_P, m_I, m_D);
		m_tal1.SetIzone(m_iZone);
		m_tal1.SetCloseLoopRampRate(m_rampRate);
		m_tal1.ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
		m_tal1.ConfigForwardLimit(m_upLim);
		//m_tal1.ConfigReverseLimit(m_lowLim);
		m_tal1.SetSensorDirection(true);
		m_tal1.SetControlMode(CANTalon::ControlMode::kPosition);
		m_tal1.SetPosition(0.0);
		calibrated = true;
		m_targetPos = m_lowLim;
	}
	else
	{
		m_tal1.SetControlMode(CANTalon::ControlMode::kPercentVbus);
		m_tal1.SetVoltageRampRate(m_rampRate);
		m_tal1.Set(-0.5);
		calibrated = false;
	}
}

void Lifter::setP(double P)
{
	m_P = P;
}

double Lifter::getP()
{
	return m_P;
}

void Lifter::setI(double I)
{
	m_I = I;
}

double Lifter::getI()
{
	return m_I;
}

void Lifter::setD(double D)
{
	m_D = D;
}

double Lifter::getD()
{
	return m_D;
}

bool Lifter::setTargetPosition(double position)
{
	if(!isCalibrated())
	{
		calibrate();
		return false;
	}

	m_targetPos = position;

	if (m_targetPos < m_lowLim)
	{
		m_targetPos = m_lowLim;
	}
	else if (m_targetPos > m_upLim)
	{
		m_targetPos = m_upLim;
	}

	m_tal1.SetCloseLoopRampRate(m_rampRate);
	m_tal1.Set(m_targetPos);
	if (getCurrentPosition() == m_targetPos)
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
	return m_targetPos;
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
	m_targetState = h;

	setTargetPosition(inchesOffGroundToTicks(States[m_targetState]));
	if (Lifter::getCurrentState() == Lifter::getTargetState())
	{
		if(Lifter::getCurrentPosition() <= 100 && m_targetState == Ground)
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
	return m_targetState;
}

Lifter::Height_t Lifter::getCurrentState()
{
	for(int i = 0; i < Top; i++)
	{
		if (Lifter::getCurrentPosition() < inchesOffGroundToTicks(States[i]))
		{
			return Height_t(i);
		}
	}
	return Height_t::Ground;
}

void Lifter::offsetTarget(double offset)	// Inches
{
	Lifter::setTargetPosition(m_targetPos + offset);
}

double Lifter::inchToTicks(double inches)
{
	return (inches * m_ticksPerInch);
}

double Lifter::inchesOffGroundToTicks(double inches)
{
	return inchToTicks((inches - m_inchesOffGround)); // Subtracting distance to ground
}
