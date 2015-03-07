#include <CameraGimbal.h>
#include "stdio.h"
#include "WPILib.h"

const float CameraGimbal::xStates[] = {
		[CameraGimbal::FRONT] = 0.0,
		[CameraGimbal::BACK] = 1.0
};

const float CameraGimbal::yStates[] = {
		[CameraGimbal::DOWN] = 0.0,
		[CameraGimbal::HORIZONTAL] = 0.5
};

CameraGimbal::CameraGimbal(int servoIDX, int servoIDY,
		float homeX, float homeY): m_servoX(servoIDX), m_servoY(servoIDY)
{
	m_homeX = homeX;
	m_homeY = homeY;
}

void CameraGimbal::setPosition(float xPos, float yPos)
{
	m_servoX.Set(xPos);
	m_servoY.Set(yPos);
}

float CameraGimbal::getPositionX()
{
	return m_servoX.Get();
}

float CameraGimbal::getPositionY()
{
	return m_servoY.Get();
}

void CameraGimbal::setHomePosition()
{
	m_servoX.Set(m_homeX);
	m_servoY.Set(m_homeY);
}

void CameraGimbal::setState(xPos_t x, yPos_t y)
{
	setPosition(xStates[x], yStates[y]);
}

void CameraGimbal::setX(xPos_t x)
{
	m_servoX.Set(xStates[x]);
}

void CameraGimbal::setY(yPos_t y)
{
	m_servoY.Set(yStates[y]);
}

void CameraGimbal::setView(cameraState_t a)
{
	switch(a) {
	case TOTE_VIEW:
	{
		CameraGimbal::setY(0.5);
		CameraGimbal::setX(0.5);
	}
	case RC_GRABBER_VIEW:
	{
		CameraGimbal::setY(0.75);
		CameraGimbal::setX(0.5);
	}
	break;
}
}

void CameraGimbal::setX(float xPos)
{
	m_servoX.Set(xPos);
}

void CameraGimbal::setY(float yPos)
{
	m_servoY.Set(yPos);
}

