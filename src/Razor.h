/*
 * Razor.h
 *
 *  Created on: Feb 17, 2015
 *      Author: Edward
 */

#ifndef SRC_RAZOR_H_
#define SRC_RAZOR_H_

#include <thread>

#include "WPILib.h"
#include "LogObject.h"
#include "Cartesian.h"

class Razor: private DRR::LogObject<Razor>
{
private:
	SerialPort *razor;
	Cartesian current;
	std::thread pollThread;
	float roll, pitch, yaw;

public:
	Razor();
	virtual ~Razor() {if(razor) delete razor;};

public:
	const Cartesian GetCurrentAcceleration() const {return current;};
	const float GetRoll() const {return roll;};
	const float GetPitch() const {return roll;};
	const float GetYaw() const {return yaw;};
	void Update();
	void Loop();
	const bool Init();
};



#endif /* SRC_RAZOR_H_ */
