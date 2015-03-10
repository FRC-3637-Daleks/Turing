#include <CameraGimbal.h>
#include "stdio.h"
#include "WPILib.h"

const float CameraGimbal::xStates[] = {
		[CameraGimbal::TOTE_VIEW] = 0.0,
		[CameraGimbal::RC_GRABBER_VIEW] = 1.0
};

const float CameraGimbal::yStates[] = {
		[CameraGimbal::TOTE_VIEW] = 0.0,
		[CameraGimbal::RC_GRABBER_VIEW] = 0.5
};


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


void CameraGimbal::setState(cameraState_t targetState)
{
		setPosition(xStates[targetState], yStates[targetState]);
}




