/*
 * ToggleInput.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: Edward
 */

#include "ToggleInput.h"

void ToggleInput::Update()
{
	const bool pressed = Pressed();
	if(pressed && !previous)
		state = !state;
	previous = pressed;
}
