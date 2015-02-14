/*
 * OperatorConsole.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: edward
 */

#include <math.h>
#include "OperatorConsole.h"

OperatorConsole::OperatorConsole(const short driveLeftID, const short driveRightID, const short copilotLeftID, const short copilotRightID, const float precision):
	m_driveLeft(driveLeftID), m_driveRight(driveRightID), m_copilotLeft(driveLeftID), m_copilotRight(driveRightID),
	precisionFactor(precision), flips({0}), squaredDrive(true), squaredCam(false), squaredLift(false), squaredBinPull(false),
	precisionEnabled(false), relativeDriveEnabled(false), steadyDriveEnabled(true), copilotMode(ROUTINE), autonMode(FORWARD)
{

}

const float OperatorConsole::convertAxis(const float raw, const bool squared, const bool flip, const double precision)
{
	if(fabs(raw) < deadzone)
		return 0.0;
	float ret = raw*precision;
	if(squared) ret *= fabs(raw);
	if(flip) ret = -ret;
	return ret *= precision;
}

void OperatorConsole::UpdateDriveControls()
{
	SetPrecisionEnabled(PollPrecisionDriving());	/// Precision Driving
	SetRelativeDriveEnabled(PollRelativeDriving());	/// Relative Driving
}

const bool OperatorConsole::GetFlip(const AnalogControls control) const
{
	if(control >= N_CONTROLS)
		return false;
	return flips[control];
}

void OperatorConsole::SetFlip(const AnalogControls control, const bool flip)
{
	if(control < N_CONTROLS)
		flips[control] = flip;
}

const float OperatorConsole::GetDriveX()
{
	return convertAxis(m_driveRight.GetX(), GetDriveSquared(), GetFlip(DRIVE_X), GetPrecisionFactor());
}



const bool OperatorConsole::PollPrecisionDriving()
{
	return m_driveRight.GetTrigger() && m_driveLeft.GetTrigger();
}

const bool OperatorConsole::PollRelativeDriving()
{
	return m_driveRight.GetRawButton(3) && m_driveLeft.GetRawButton(3);
}




