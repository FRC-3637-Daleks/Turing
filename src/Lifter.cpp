#include "Lifter.h"
#include "stdio.h"
#include "WPILib.h"

// States are in Inches
const double Lifter::States[] = {
		3.0, 8.5, 12.0, 13.5, 18.0, 19.0, 20.0, 24.5, 26.0 // BinT1 is tentative
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
		m_tal1.SelectProfileSlot(0);
		m_tal1.SetFeedbackDevice(CANTalon::QuadEncoder);
		/*m_tal1.SetPID(m_P, m_I, m_D);
		m_tal1.SetIzone(m_iZone);
		m_tal1.SetCloseLoopRampRate(m_rampRate);
		*/
		m_tal1.ConfigLimitMode(CANSpeedController::kLimitMode_SoftPositionLimits);
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

	if (m_targetState == Ground)
	{
		Lifter::offsetTarget(-100.0);
		if(!m_tal1.GetReverseLimitOK())
		{
			calibrate();
		}
	}
	else
	{
		setTargetPosition(inchesOffGroundToTicks(States[m_targetState]));
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
	return m_targetState;
}

Lifter::Height_t Lifter::getCurrentState()
{
	for(int i = 0; i < 5; i++)
	{
		if (Lifter::getCurrentPosition() < States[i])
		{
			switch(i)
			{
			case 0:
				return Height_t::Ground;
				break;
			case 1:
				return Height_t::Step;
				break;
			case 2:
				return Height_t::StickUp;
				break;
			case 3:
				return Height_t::Bin0;
				break;
			case 4:
				return Height_t::Bin1;
				break;
			case 5:
				return Height_t::BinUp;
				break;
			case 6:
				return Height_t::Holding;
				break;
			case 7:
				return Height_t::ToteUp;
				break;
			case 8:
				return Height_t::BinT1;
				break;
			default:
				return Height_t::Ground;
			}
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
