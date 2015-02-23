/*
 * OperatorConsole.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: edward
 */

#include <math.h>
#include "OperatorConsole.h"

OperatorConsole::OperatorConsole(const short driveLeftID, const short driveRightID, const short copilotLeftID, const short copilotRightID, const float precision):
	m_driveLeft(driveLeftID), m_driveRight(driveRightID), m_copilotLeft(copilotLeftID), m_copilotRight(copilotRightID),
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
	if(GetPrecisionEnabled())
		SetDriveSquared(false);
	else
		SetDriveSquared(true);
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
	return convertAxis(m_driveRight.GetX(), GetDriveSquared(), GetFlip(DRIVE_X), 1.0);
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
	return convertAxis(m_copilotLeft.GetAxis(GamePad::PadAxisType::LEFT_X), GetCamSquared(), GetFlip(AnalogControls::CAM_Y))/2.0+0.5;
}

const float OperatorConsole::GetCamY()
{
	return convertAxis(m_copilotLeft.GetAxis(GamePad::PadAxisType::LEFT_Y), GetCamSquared(), GetFlip(AnalogControls::CAM_Y))/2.0+0.5;
}

const bool OperatorConsole::PollPrecisionDriving()
{
	if(m_driveRight.GetRawButton(10) && GetPrecision() == true)
		return false;
	else if(m_driveRight.GetRawButton(11) && GetPrecision() == false)
		return true;
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
		if(targetHeight >= Lifter::Top)
			targetHeight = Lifter::Top;
	}

	if(!liftDown && m_copilotLeft.GetButton(GamePad::BOTTOM_RIGHT_SHOULDER))
	{
		liftDown = true;
		targetHeight = Lifter::Height_t(int(targetHeight)-1);
		if(targetHeight <= Lifter::Ground)
			targetHeight = Lifter::Ground;
	}
}

const bool OperatorConsole::GetHoldExtend()
{
	return m_copilotLeft.GetButton(GamePad::B4);
}

const bool OperatorConsole::GetHoldRetract()
{
	return m_copilotLeft.GetButton(GamePad::B2);
}

const float OperatorConsole::GetLift()
{
	return 4.0*convertAxis(m_copilotLeft.GetAxis(GamePad::RIGHT_Y), squaredLift, flips[LIFT], 1.0);
}

const bool OperatorConsole::GetGround()
{
	return m_copilotLeft.GetButton(GamePad::B2);
}

const bool OperatorConsole::GetPushTote()
{
	return m_copilotLeft.GetButton(GamePad::B3);
}

const bool OperatorConsole::GetScoreStep()
{
	return m_copilotLeft.GetButton(GamePad::B4);
}

const bool OperatorConsole::GetScore()
{
	return m_copilotLeft.GetButton(GamePad::B1);
}

const bool OperatorConsole::GetLifterAuto()
{
	return m_copilotLeft.GetButton(GamePad::TOP_RIGHT_SHOULDER);
}
