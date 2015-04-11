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
		razor = new SerialPort(57600, SerialPort::kUSB);
		razor->EnableTermination();
		pollThread = std::thread(&Razor::Loop, this);
	}
	return true;
}

void Razor::Update()
{
	char buf[255] = {};
	while(razor->GetBytesReceived())
		razor->Read(buf, 255); // Gets most recent line
	//printf(buf);
	LogText(DRR::LEVEL_t::DEBUG2)<<buf;
	sscanf(buf, "#YPR:%f,%f,%f\n", &yaw, &pitch, &roll);
	razor->Reset();
}

void Razor::Loop()
{
	while(true)
	{
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

