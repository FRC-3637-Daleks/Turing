/*
 * RGB.h
 *
 *  Created on: Apr 10, 2015
 *      Author: 3637-DS-1
 */

#ifndef SRC_RGB_H_
#define SRC_RGB_H_

#include "WPILib.h"
#include "LogObject.h"

class LightController: public DRR::LogObject<LightController>
{
private:
	DigitalOutput rOut, gOut, bOut;
	unsigned short rVal, gVal, bVal;

public:
	LightController(const short rPort, const short gPort, const short bPort);

private:
	void Refresh();

public:
	void SetR(const unsigned short val);
	void SetG(const unsigned short val);
	void SetB(const unsigned short val);

	const unsigned short GetR() const {return rVal;};
	const unsigned short GetG() const {return gVal;};
	const unsigned short GetB() const {return bVal;};
};



#endif /* SRC_RGB_H_ */
