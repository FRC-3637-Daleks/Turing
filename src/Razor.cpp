/*
 * Razor.cpp
 *
 *  Created on: Feb 17, 2015
 *      Author: Edward
 */

#include "Razor.h"

Razor::Razor(): razor(NULL), current(), roll(0.0), pitch(0.0), yaw(0.0)
{
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
	int x, y, z;
	sscanf(buf, "!ANG:%f,%f,%f,ACC:%d,%d,%d\n", &roll, &pitch, &yaw, &y, &x, &z);
	current(double(x), double(y), double(z));
	razor->Reset();
}

