/*
 * Camera.h
 *
 *  Created on: Feb 10, 2015
 *      Author: elija_000
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "WPILib.h"
#include "Hardware.h"

class Servo;

class Camera {
public:
	Camera(uint32_t ServoX, uint32_t ServoY);
	Camera(Servo &ServoX, Servo &ServoY);
	Camera(Servo *ServoX, Servo *ServoY);

	enum Camera_t { CAMERA_NEW_XY_TARGET, CAMERA_AT_XY_TARGET,
					NUM_STATES };  // Go to (x,y)

	void setPosition(Camera_t p);
	Camera_t getPosition();
	Camera_t Pan();
	Camera_t Tilt();

private:
		Servo *c_x;
		Servo *c_y;
		bool m_needFree;
		Camera_t m_currentState;
};

#endif
