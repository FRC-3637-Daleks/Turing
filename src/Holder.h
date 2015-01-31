/*
 * Holder.h
 *
 *  Created on: Jan 31, 2015
 *      Author: elija_000
 */
#ifndef _HOLDER_H_
#define _HOLDER_H_

// This file should only be included in Holder.cpp

#include "WPILib.h"

#include "Hardware.h"

#include "Logger.h"
#include "SmartDashService.h"

class Holder {
	public:
		Holder(Solenoid InSolenoid, Solenoid OutSolenoid);

		enum {PISTON_IN,PISTON_OUT};

		static const char
};

#endif




