/*
 * Camera.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: elija_000
 */
#include "Camera.h"
#include "WPILib.h"

Camera::Camera(uint32_t ServoX, uint32_t ServoY)
{
	c_x = new Servo(ServoX);
	c_y = new Servo(ServoY);
	m_needFree = true;
	return;
}

Camera::Camera(Servo &ServoX, Servo &ServoY)
{
	c_x = &ServoX;
	c_y = &ServoY;
	m_needFree = false;
	return;
}

Camera::Camera(Servo *ServoX, Servo *ServoY)
{
	c_x = ServoX;
	c_y = ServoY;
	m_needFree = false;
	return;
}

camera_t
Camera::setPanPosition(camera_t p, float PanTarget) // adjusts camera pan based on value of p
{
	IF (p=CAMERA_NEW_TARGET):
			pan(PanTarget);
			break;
	}
	return CAMERA_AT_TARGET ;
}

camera_t
Camera::setTiltPosition(camera_t p, float TiltTarget) // adjusts camera pan based on value of p
{
	IF (p=CAMERA_NEW_TARGET):
		tilt(TiltTarget);
			break;
	}
	return CAMERA_AT_TARGET ;
}


/*
Camera::camera_t
Camera::getPosition()
{

	if (c_x->Get())
		m_cameraState = CAMERA_IN;
	else
		m_cameraState = CAMERA_OUT;
	//m_cameraState = (c_x->Get() ? CAMERA_IN : CAMERA_OUT);
	return m_cameraState;
} */


Camera::pan(float panTarget)
{
	c_x=panTarget;
	return CAMERA_AT_TARGET;
}


Camera::tilt(float tiltTarget)
{
	c_y=tiltTarget;
	return CAMERA_AT_TARGET;
}




