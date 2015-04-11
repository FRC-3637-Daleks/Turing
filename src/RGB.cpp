/*
 * RGB.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: 3637-DS-1
 */

#include "RGB.h"

LightController::LightController(const short rPort, const short gPort, const short bPort):
								rOut(rPort), gOut(gPort), bOut(bPort), rVal(0), bVal(0), gVal(0)
{
	Refresh();
	AddLog("red", &LightController::GetR, 0);
	AddLog("green", &LightController::GetG, 0);
	AddLog("blue", &LightController::GetB, 0);

}

void LightController::Refresh()
{
	rOut.Set(rVal);
	gOut.Set(gVal);
	bOut.Set(bVal);
}

void LightController::SetR(const unsigned short val)
{
	if(val > 255) rVal = 255;
	else 		  rVal = val;
	Refresh();
}

void LightController::SetG(const unsigned short val)
{
	if(val > 255) gVal = 255;
	else 		  gVal = val;
	Refresh();
}

void LightController::SetB(const unsigned short val)
{
	if(val > 255) bVal = 255;
	else 		  bVal = val;
	Refresh();
}
