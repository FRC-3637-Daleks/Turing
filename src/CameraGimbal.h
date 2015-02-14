
#ifndef SRC_CAMERAGIMBAL_H_
#define SRC_CAMERAGIMBAL_H_

#include "WPILib.h"

class CameraGimbal
{
private:
	Servo m_servoX, m_servoY;
	float m_homeX, m_homeY;
public:
	CameraGimbal(int servoIDX, int servoIDY, float homeX, float homeY);
	void setPosition(float xPos, float yPos);
	float getPositionX();
	float getPositionY();
	void setHomePosition();
};



#endif /* SRC_CAMERAGIMBAL_H_ */
