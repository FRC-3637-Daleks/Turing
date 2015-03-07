/*
 * Aligners.h
 *
 *  Created on: Mar 8, 2015
 *      Author: 3637-DS-1
 */

#ifndef SRC_ALIGNERS_H_
#define SRC_ALIGNERS_H_

#include "WPILib.h"

class Aligners
{
public:
	typedef enum {OUT, IN} POSITION_t;

private:
	Servo m_left, m_right;
	POSITION_t target;

public:
	Aligners(const unsigned int leftID, const unsigned int rightID): m_left(leftID), m_right(rightID) {Retract();};

public:
	void SetPosition(const float f);
	void Retract();
	void Extend();
};



#endif /* SRC_ALIGNERS_H_ */
