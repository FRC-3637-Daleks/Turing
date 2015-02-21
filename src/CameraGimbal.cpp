#include <CameraGimbal.h>
#include "stdio.h"
#include "WPILib.h"

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
