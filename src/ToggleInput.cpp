/*
 * ToggleInput.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: Edward
 */

#include "ToggleInput.h"

void FlickInput::Update()
{
	const bool pressed = Pressed();
	if(pressed && !previous)
		SetState(true);
	else
		SetState(false);

	previous = pressed;
}

void ToggleInput::Update()
{
	const bool pressed = Pressed();
	if(pressed && !previous)
		SetState(!GetState());
	previous = pressed;
}
