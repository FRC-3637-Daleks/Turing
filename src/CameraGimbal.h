
#ifndef SRC_CAMERAGIMBAL_H_
#define SRC_CAMERAGIMBAL_H_

#include "WPILib.h"

class CameraGimbal
{
public:
	enum xPos_t{FRONT = 0, BACK};
	enum yPos_t{DOWN = 0, HORIZONTAL};
	static const float xStates[];
	static const float yStates[];
private:
	Servo m_servoX, m_servoY;
	float m_homeX, m_homeY;
public:
	CameraGimbal(int servoIDX, int servoIDY, float homeX, float homeY);
	void setPosition(float xPos, float yPos);
	float getPositionX();
	float getPositionY();
	void setHomePosition();
	void setState(xPos_t x, yPos_t y);
	void setX(xPos_t x);
	void setY(yPos_t y);
	/* Not sure which one you wanted
	void setX(float xPos);
	void setY(float yPos);
	*/
};



#endif /* SRC_CAMERAGIMBAL_H_ */
