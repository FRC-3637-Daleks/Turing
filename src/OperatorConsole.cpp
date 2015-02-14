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
	liftUp(false), liftDown(false),
	precisionEnabled(false), relativeDriveEnabled(false), steadyDriveEnabled(true), copilotMode(ROUTINE), autonMode(FORWARD),
	targetHeight(Lifter::Ground)
{

}

const float OperatorConsole::GetPrecision() const
{
	if(GetPrecisionEnabled())
		return GetPrecisionFactor();
	else
		return 1.0;
}

const float OperatorConsole::convertAxis(const float raw, const bool squared, const bool flip, const double precision)
{
	if(fabs(raw) < deadzone)
		return 0.0;
	float ret = raw;
	if(squared) ret *= fabs(raw);
	if(flip) ret = -ret;
	return ret*precision;
}

void OperatorConsole::UpdateDriveControls()
{
	SetPrecisionEnabled(PollPrecisionDriving());	/// Precision Driving
	SetRelativeDriveEnabled(PollRelativeDriving());	/// Relative Driving
	PollLifterHeight();
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
	return convertAxis(m_driveRight.GetX(), GetDriveSquared(), GetFlip(DRIVE_X), GetPrecision());
}

const float OperatorConsole::GetDriveY()
{
	return convertAxis(m_driveRight.GetY(), GetDriveSquared(), GetFlip(DRIVE_Y), GetPrecision());
}

const float OperatorConsole::GetDriveYaw()
{
	return convertAxis(m_driveLeft.GetX(), GetDriveSquared(), GetFlip(DRIVE_YAW), GetPrecision());
}

const float OperatorConsole::GetCamX()
{
	return convertAxis(m_copilotLeft.GetAxis(GamePad::PadAxisType::LEFT_X));
}

const float OperatorConsole::GetCamY()
{
	return convertAxis(m_copilotLeft.GetAxis(GamePad::PadAxisType::LEFT_Y));
}

const bool OperatorConsole::PollPrecisionDriving()
{
	return m_driveRight.GetTrigger() && m_driveLeft.GetTrigger();
}

const bool OperatorConsole::PollRelativeDriving()
{
	return m_driveRight.GetRawButton(3) && m_driveLeft.GetRawButton(3);
}

void OperatorConsole::PollLifterHeight()
{
	if(!m_copilotLeft.GetButton(GamePad::TOP_RIGHT_SHOULDER) && !m_copilotLeft.GetButton(GamePad::BOTTOM_RIGHT_SHOULDER))
		liftUp = liftDown = false;

	if(!liftUp && m_copilotLeft.GetButton(GamePad::TOP_RIGHT_SHOULDER))
	{
		liftUp = true;
		targetHeight = Lifter::Height_t(int(targetHeight)+1);
		if(targetHeight >= Lifter::BinT1)
			targetHeight = Lifter::BinT1;
	}

	if(!liftDown && m_copilotLeft.GetButton(GamePad::BOTTOM_RIGHT_SHOULDER))
	{
		liftDown = true;
		targetHeight = Lifter::Height_t(int(targetHeight)-1);
		if(targetHeight <= Lifter::Ground)
			targetHeight = Lifter::Ground;
	}
}


