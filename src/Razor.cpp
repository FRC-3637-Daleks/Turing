/*
 * Razor.cpp
 *
 *  Created on: Feb 17, 2015
 *      Author: Edward
 */

#include "Razor.h"

Razor::Razor(): razor(NULL), current(), roll(0.0), pitch(0.0), yaw(0.0)
{
	LogText()<<"Constructing";
	AddLog<float>("roll", [this]() -> float {return roll;}, 0);
	AddLog<float>("pitch", [this]() -> float {return pitch;}, 0);
	AddLog<float>("yaw", [this]() -> float {return yaw;}, 0);
}

const bool Razor::Init()
{
	if(razor == NULL)
	{
		razor = new SerialPort(19200, SerialPort::kUSB);
		razor->EnableTermination();
	}
	return true;
}

void Razor::Update()
{
	static char buf[255];
	while(razor->GetBytesReceived())
		razor->Read(buf, 255); // Gets most recent line
	//printf(buf);
	sscanf(buf, "!ANG:%f,%f,%f\n", &yaw, &pitch, &roll);
	razor->Reset();
}

