/*
 * Sweeper.h
 *
 *  Created on: Feb 14, 2015
 *      Author: elija_000
 */

#ifndef SRC_SWEEPER_H_
#define SRC_SWEEPER_H_

#include "WPILib.h"
#include "Hardware.h"

class Talon;

class DigitalInput;

class Sweeper {

private:
		Talon *m_motor;
		bool m_needFree;
};

#endif
