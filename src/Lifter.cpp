#include "Lifter.h"
#include "stdio.h"
#include "WPILib.h"

const double Height_t::States[] = {
		0.0, 1.0, 1.5, 2.0, 3.0
};

Lifter::Lifter(CANTalon tal1, CANTalon tal2,
		double P, double I, double D,
		double upLim, double lowLim, Holder h)
{
	m_tal1 = tal1;
	m_tal2 = tal2;
	m_P = P;
	m_I = I;
	m_D = D;
	m_upLim = upLim;
	m_lowLim = lowLim;
	m_h = h;
	m_targetPos = 0.0;
	m_targetState = Height_t::Ground;
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
	m_targetPos = position;
	m_tal1.SetPosition(position);
	if (getCurrentPosition() == position)
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
	int i;
	switch(m_targetState)
	{
	case Height_t::Ground:
		i = 0;
		break;
	case Height_t::Tote1:
		i = 1;
		break;
	case Height_t::ToteCan:
		i = 2;
		break;
	case Height_t::Tote2:
		i = 3;
		break;
	case Height_t::Tote3:
		i = 4;
		break;
	default:
		i = 0;
	}

	m_tal1.SetPosition(Height_t::States[i]);

	if (Lifter::getCurrentState() == Lifter::getTargetState())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Height_t Lifter::getTargetState()
{
	return m_targetState;
}

Height_t Lifter::getCurrentState()
{
	for(int i = 0; i < 5; i++)
	{
		if (Lifter::getCurrentPosition() < Height_t::States[i])
		{
			switch(i)
			{
			case 0:
				return Height_t::Ground;
				break;
			case 1:
				return Height_t::Tote1;
				break;
			case 2:
				return Height_t::ToteCan;
				break;
			case 3:
				return Height_t::Tote2;
				break;
			case 4:
				return Height_t::Tote3;
				break;
			default:
				return Height_t::Tote3;
			}
		}
	}
	return Height_t::Tote3;
}
