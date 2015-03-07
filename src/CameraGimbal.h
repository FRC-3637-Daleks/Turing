
#ifndef SRC_CAMERAGIMBAL_H_
#define SRC_CAMERAGIMBAL_H_

#include "WPILib.h"

class CameraGimbal
{
public:

	enum cameraState_t {TOTE_VIEW, RC_GRABBER_VIEW};
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
	void setState(cameraState_t targetState);



	void setX(float xPos);
	void setY(float yPos);

};



#endif /* SRC_CAMERAGIMBAL_H_ */
