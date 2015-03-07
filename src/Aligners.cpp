/*
 * Aligners.cpp
 *
 *  Created on: Mar 8, 2015
 *      Author: 3637-DS-1
 */

#include "Aligners.h"

void
Aligners::SetPosition(const float f)
{
	m_left.Set(f);
	m_right.Set(1.0-f);
}

void
Aligners::Retract()
{
	target = IN;
	SetPosition(0.0);
}

void
Aligners::Extend()
{
	target = OUT;
	SetPosition(0.5);
}
