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

class OperatorConsole
{
public:
	enum CopilotMode_t {MANUAL, HEIGHT, ROUTINE};
	enum AnalogControls {DRIVE_X=0, DRIVE_Y, DRIVE_YAW, CAM_X, CAM_Y, LIFT, BIN_PULL, N_CONTROLS};
	enum AutonomousMode_t {FORWARD, TOTE_SET, CONTAINER_SET, TOTE_STACK};

private:
	static constexpr double deadzone = 0.05;

private:	/// Hardware
	Joystick m_driveLeft, m_driveRight;
	GamePad m_copilotLeft;		///< Actual joystick type unknown
	Joystick m_copilotRight;		///< Actual joystick type unknown

private:	/// Configuration
	float precisionFactor;
	bool flips[AnalogControls::N_CONTROLS];
	bool squaredDrive, squaredCam, squaredLift, squaredBinPull;		///< Axis squaring

private:	/// Button states
	bool liftUp, liftDown;

private:	/// Dynamic settings
	bool precisionEnabled, relativeDriveEnabled, steadyDriveEnabled;
	CopilotMode_t copilotMode;
	AutonomousMode_t autonMode;
	Lifter::Height_t targetHeight;

public:
	OperatorConsole(const short driveLeftID, const short driveRightID, const short copilotLeftID, const short copilotRightID, const float precision=0.3);
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

	const AutonomousMode_t GetAutonomousMode() const {return autonMode;};
	void SetAutonomousMode(const AutonomousMode_t mode) {autonMode = mode;};

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

	const CopilotMode_t GetCopilotMode() const {return copilotMode;};
	void SetCopilotMode(const CopilotMode_t mode) {copilotMode = mode;};

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
	const bool PollPrecisionDriving();
	const bool PollRelativeDriving();
	void PollLifterHeight();

public:		/// Button State Functions
	const bool GetCenterCamera();
	/*...*/
};



#endif /* SRC_OPERATORCONSOLE_H_ */