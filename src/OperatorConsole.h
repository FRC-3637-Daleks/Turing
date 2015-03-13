/*
 * OperatorConsole.h
 *
 *  Created on: Feb 13, 2015
 *      Author: edward
 */

#ifndef SRC_OPERATORCONSOLE_H_
#define SRC_OPERATORCONSOLE_H_

#include "Hardware.h"
#include "Holder.h"
#include "Lifter.h"
#include "DalekDrive.h"

#include "Gamepad.h"
#include "ToggleInput.h"
#include "Sweeper.h"

class OperatorConsole
{
public:
	enum CopilotMode_t {MANUAL, HEIGHT, ROUTINE};
	enum AnalogControls {DRIVE_X=0, DRIVE_Y, DRIVE_YAW, CAM_X, CAM_Y, LIFT, BIN_PULL, N_CONTROLS};

private:
	static constexpr double deadzone = 0.1;

private:	/// Hardware
	Joystick m_driveLeft, m_driveRight;
	GamePad m_copilotLeft;		///< Actual joystick type unknown
	Joystick m_copilotRight;		///< Actual joystick type unknown

private:	/// Configuration
	float precisionFactor;
	bool flips[AnalogControls::N_CONTROLS];
	bool squaredDrive, squaredCam, squaredLift, squaredBinPull;		///< Axis squaring

private:	/// Button states
	ToggleInput manualToggle, preciseToggle;
	FlickInput upFlick, downFlick;
	BoolInput *inputManagers[5];
	bool failsafe;

private:	/// Dynamic settings
	bool precisionEnabled, relativeDriveEnabled, steadyDriveEnabled;
	Lifter::Height_t targetHeight;

public:
	OperatorConsole(const short driveLeftID, const short driveRightID, const short copilotLeftID, const short copilotRightID, const float precision=0.55);
	virtual ~OperatorConsole() {};

private:
	static const float convertAxis(const float raw, const bool squared=true, const bool flip=false, const double precision=1.0);

public:
	/// Updates controls for when humans are driving
	void UpdateDriveControls();

	/// Updates controls during disabled mode
	void UpdatePreGame() {};

public:	/// Class internal status Get and Set functions
	const float GetPrecisionFactor() const {return precisionFactor;};
	const float GetPrecision() const;
	const bool GetFlip(const AnalogControls control) const;
	void SetFlip(const AnalogControls control, const bool flip);

	const bool GetDriveSquared() const {return squaredDrive;};
	const bool GetCamSquared() const {return squaredCam;};
	const bool GetLiftSquared() const {return squaredLift;};
	const bool GetBinPullSquared() const {return squaredBinPull;};
	void SetDriveSquared(const bool enabled) {squaredDrive = enabled;};
	void SetCamSquared(const bool enabled) {squaredCam = enabled;};
	void SetLiftSquared(const bool enabled) {squaredLift = enabled;};
	void SetBinPullSquared(const bool enabled) {squaredBinPull = enabled;};

	const bool GetPrecisionEnabled() const {return precisionEnabled;};
	void SetPrecisionEnabled(const bool enabled) {precisionEnabled = enabled;};
	void EnabledPrecision() {SetPrecisionEnabled(true);};
	void DisablePrecision() {SetPrecisionEnabled(false);};

	const bool GetRelativeDriveEnabled() const {return relativeDriveEnabled;};
	void SetRelativeDriveEnabled(const bool enabled) {relativeDriveEnabled = enabled;};
	void EnabledRelativeDrive() {SetRelativeDriveEnabled(true);};
	void DisableRelativeDrive() {SetRelativeDriveEnabled(false);};

	const bool GetSteadyDriveEnabled() const {return steadyDriveEnabled;};
	void SetSteadyDriveEnabled(const bool enabled) {steadyDriveEnabled = enabled;};
	void EnabledSteadyDrive() {SetSteadyDriveEnabled(true);};
	void DisableSteadyDrive() {SetSteadyDriveEnabled(false);};

	const bool GetManual() const {return manualToggle.GetState();};

	const Lifter::Height_t GetLiftTarget() const {return targetHeight;};

public: /// Analog Get Functions
	const float GetDriveX();
	const float GetDriveY();
	const float GetDriveYaw();
	const float GetCamX();
	const float GetCamY();
	const float GetLift();  ///< Returns 0 if not in manual mode
	const float GetBinPull();

private:	/// Private Button State Poll Functions
	const bool PollRelativeDriving();
	void PollLifterHeight();

public:		/// Button State Functions
	const Sweeper::State_t GetSweeperState();
	const Sweeper::Mode_t GetSweeperMode();
	const bool GetReset();
	const bool GetCenterCamera();
	const bool GetGround();
	const bool GetAlignExtend();

public:	/// Auto Mode
	const int GetAutoMode();

};



#endif /* SRC_OPERATORCONSOLE_H_ */
