/*
* Holder.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: elija_000

 Holder

Holder() - constructs Holder

enum Position_t PISTON_OUT, PISTON_IN

setPosition(Position_t p) - either extends or retracts pistons based on value of p

Position_t getPosition() - returns current state of pistons (in or out)

retract() - retracts pistons

extend() - extends pistons

 */
//#include "Holder.h"

using namespace std;


void Holder()
{

	Solenoid1 = new Solenoid(1);

	int currentPosition = getPosition();  //set currentPosition to current position of holder

	setPosition(currentPosition, desiredPosition); /*either extends or retracts pistons based on value of desired position*/

	// 0 = holder is in retract position
	// 1 = holder is in extend position

	if (currentPosition == desiredPosition){
	}else if (desiredPosition == 1){ //if desiredPosition is to extend then extend
		Solenoid1.set(true);//extend();
	}else if (desiredPosition == 0){ //if desiredPosition is to retract then retract
		//Holder.set(DoubleSolenoid.Value.kReverse)//retract();
		Solenoid1.set(false);
	}

void extend()
{

}

void retract()
{

}

};


