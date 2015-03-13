/*
 * DalekDrive.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: Edward
 */

#include "DalekDrive.h"
#include "CAN/can_proto.h"

float leftFrontAlg(float x, float y, float theta) {return x+y+theta;}
float rightFrontAlg(float x, float y, float theta) {return y-x-theta;}
float leftRearAlg(float x, float y, float theta) {return y-x+theta;}
float rightRearAlg(float x, float y, float theta) {return y+x-theta;}

DalekDrive::Motor::MECANUM_FUNCTION DalekDrive::Motor::mecFuncs[] = {leftFrontAlg, rightFrontAlg, leftRearAlg, rightRearAlg};



DalekDrive::Motor &DalekDrive::Motor::operator =(Motor m)
// swaps resources and allows the local parameter to safely destruct them
{
	std::swap(motor, m.motor);
	std::swap(refCount, m.refCount);
	refCount? ++*refCount:NULL;
	std::swap(flip, m.flip);
	std::swap(location, m.location);

	return *this;
}

DalekDrive::Motor::~Motor()
{
	if(refCount)  // sees if this has a reference counter pointer
	{
		if(!--*refCount && motor)  // if it does it subtracts one from it and checks to see if it's the last one
			delete motor;   // delets motor if necessary
		delete refCount;
	}
}


void DalekDrive::init(const Wheel_t wheelConfig, const DalekDrive::Motor m [], const float ramp, const int syncGroup)
{
	LogText()<<"Initializing DalekDrive";
	this->syncGroup = syncGroup;
	printMotors();
	wheels = wheelConfig;
	for(short i = 0; i < N_MOTORS; i++)
	{
		motors[i] = m[i];
		motors[i]->SetVoltageRampRate(ramp);
	}
	printMotors();
}

DalekDrive::DalekDrive(const Wheel_t wheelConfig, const DalekDrive::Motor m[],const float ramp, const int syncGroup): LogObject<DalekDrive>(this)
{
	init(wheelConfig, m, ramp, syncGroup);
}

DalekDrive::DalekDrive(const Wheel_t wheelConfig,
		const Motor & leftFront,
		const Motor & rightFront,
		const Motor & leftRear,
		const Motor & rightRear,
		const float ramp,
		const int syncGroup): LogObject<DalekDrive>(this)
{
	printMotors();
	Motor ms[] = {leftFront, rightFront, leftRear, rightRear};
	init(wheelConfig, ms, ramp, syncGroup);
	printMotors();
	// calls alternate constructor with parameters rearranged
}

DalekDrive::DalekDrive(const Wheel_t wheelConfig, const short leftFront, const short rightFront, const short leftRear, const short rightRear, const float ramp, const int syncGroup): LogObject<DalekDrive>(this)
{
	Motor ms[] = {Motor(leftFront, LEFT_FRONT, syncGroup), Motor(rightFront, RIGHT_FRONT, syncGroup), Motor(leftRear, LEFT_REAR, syncGroup), Motor(rightRear, RIGHT_REAR, syncGroup)};
	init(wheelConfig, ms, ramp, syncGroup);
	// calls alternate constructor with paramaters rearranged
}

void DalekDrive::Drive(const float x, const float y, const float theta)
{
	if(wheels == MECANUM_WHEELS)
	{
		for(short i = 0; i < N_MOTORS; motors[i++].SetMecSpeed(x, y, theta));
	}
	else
	{
		for(short i = 0; i < N_MOTORS; motors[i++].SetMecSpeed(0.0, y, theta));
	}
}

void DalekDrive::Drive(const float left, const float right)
{
	for(short i = 0; i < N_MOTORS; i++)
		motors[i].SetSpeed(i == LEFT_FRONT || i == RIGHT_FRONT? left:right);
}

void DalekDrive::UpdateSyncGroup()
{
	CANJaguar::UpdateSyncGroup(syncGroup);
}

void DalekDrive::printMotors()
{

}
