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
	manualToggle(std::bind(&GamePad::GetButton, &m_copilotLeft, GamePad::START), true),
	preciseToggle([this]() -> bool {return m_driveLeft.GetRawButton(1) && m_driveRight.GetRawButton(1);}),
	upFlick([this]() {return m_copilotLeft.GetAxis(GamePad::RIGHT_Y) > 0.5;}),
	downFlick([this]() {return m_copilotLeft.GetAxis(GamePad::RIGHT_Y) < -0.5;}),
	inputManagers({&manualToggle, &preciseToggle, &upFlick, &downFlick, NULL}),
	failsafe(false), precisionEnabled(false), relativeDriveEnabled(false), steadyDriveEnabled(true),
	targetHeight(Lifter::Ground)
{
	LogText()<<"Initialized a lot of stuff";
	Joystick *joys[4] = {&m_driveLeft, &m_driveRight, &m_copilotLeft, &m_copilotRight};
	for(int i = 0; i < 4; i++)
	{
		const std::string joy = DRR::LogService::AddID("joystick", i);
		for(int a = 0; a < 6; a++)
		{
			AddLog<float>(DRR::LogService::MakeKey(joy, DRR::LogService::AddID("axis", a)), std::bind(&Joystick::GetRawAxis, joys[i], a));
		}

		for(int b = 1; b <= 10; b++)
		{
			AddLog<bool>(DRR::LogService::MakeKey(joy, DRR::LogService::AddID("button", b)), std::bind(&Joystick::GetRawButton, joys[i], b));
		}
	}
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
	for(int i = 0; inputManagers[i] != NULL; i++)
		inputManagers[i]->Update();

	SetPrecisionEnabled(preciseToggle.GetState());	/// Precision Driving
	if(GetPrecisionEnabled())
		SetDriveSquared(false);
	else
		SetDriveSquared(true);
	//SetRelativeDriveEnabled(PollRelativeDriving());	/// Relative Driving
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
	return convertAxis(m_driveRight.GetY(), GetDriveSquared(), GetFlip(DRIVE_Y), GetPrecision()*GetPrecision());
}

const float OperatorConsole::GetDriveYaw()
{
	return convertAxis(m_driveLeft.GetX(), GetDriveSquared(), GetFlip(DRIVE_YAW), GetPrecision()*GetPrecision());
}

const float OperatorConsole::GetCamX()
{
	return convertAxis(m_copilotLeft.GetAxis(GamePad::PadAxisType::LEFT_X), GetCamSquared(), GetFlip(CAM_X))/2.0+0.5;
}

const float OperatorConsole::GetCamY()
{
	return convertAxis(m_copilotLeft.GetAxis(GamePad::PadAxisType::LEFT_Y), GetCamSquared(), GetFlip(CAM_Y))/2.0+0.5;
}

const bool OperatorConsole::PollRelativeDriving()
{
	return m_driveRight.GetRawButton(3) && m_driveLeft.GetRawButton(3);
}

void OperatorConsole::PollLifterHeight()
{
	if(upFlick.GetState() && targetHeight < Lifter::Top)
		targetHeight = Lifter::Height_t(int(targetHeight)+1);
	if(downFlick.GetState() && targetHeight > Lifter::Ground)
		targetHeight = Lifter::Height_t(int(targetHeight)-1);
}


const float OperatorConsole::GetLift()
{
	return 4.0*convertAxis(m_copilotLeft.GetAxis(GamePad::RIGHT_Y), squaredLift, flips[LIFT], 1.0);
}

const float OperatorConsole::GetBinPull()
{
	return convertAxis(m_copilotLeft.GetAxis(GamePad::LEFT_Y), GetBinPullSquared(), GetFlip(BIN_PULL));
}

const bool OperatorConsole::GetReset()
{
	return m_copilotLeft.GetButton(GamePad::LEFT_JOY_BUTTON);
}


const bool OperatorConsole::GetGround()
{
	return m_copilotLeft.GetButton(GamePad::B2);
}

const bool OperatorConsole::GetChute()
{
	return m_copilotLeft.GetButton(GamePad::B1);
}

const bool OperatorConsole::GetCenterCamera()
{
	return false;
}

const bool OperatorConsole::GetAlignExtend()
{
	return m_driveRight.GetRawButton(2) && m_driveLeft.GetRawButton(2);
}

const Cobra::State_t OperatorConsole::GetSweeperState()
{
	if(m_copilotLeft.GetButton(GamePad::TOP_LEFT_SHOULDER))
		return Cobra::Up;
	else if(m_copilotLeft.GetButton(GamePad::BOTTOM_LEFT_SHOULDER))
		return Cobra::Down;
	else if(fabs(GetBinPull()) < deadzone)
		return Cobra::Hold;
	return Cobra::Transition;
}

const Cobra::Mode_t OperatorConsole::GetSweeperMode()
{
	if(m_copilotLeft.GetButton(GamePad::START))
	{
		failsafe = false;
		return Cobra::Velocity;
	}
	else if(failsafe || m_copilotLeft.GetButton(GamePad::SELECT))
	{
		failsafe = true;
		return Cobra::RawVoltage;
	}
	else
		return Cobra::Velocity;
}

const int OperatorConsole::GetAutoMode()
{
	int i;
	for(i = 1; i < 10; i++)
	{
		if(m_copilotLeft.GetRawButton(i))
			return i;
	}

	return 0;
}

