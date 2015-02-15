#include "Turing.h"

using namespace std;

class Turing: public IterativeRobot
{
private:
	PowerDistributionPanel PDP;
	DalekDrive drive;
	Compressor compressor;
	Lifter lift;
	Holder hold;
	OperatorConsole op;
	CameraGimbal gimbal;

public:
	Turing(): drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT, 50.0),
			  lift(Robot::LIFT_1, Robot::LIFT_2, Lifter::PIDConfig(2.0, 0.005, 0.0, 20), 50.0),
			  hold(Robot::HOLDER_RETRACT, Robot::HOLDER_EXTEND, Robot::TOTE_SWITCH),
			  op(Robot::DRIVER_LEFT, Robot::DRIVER_RIGHT, Robot::COPILOT_LEFT, Robot::COPILOT_RIGHT),
			  gimbal(Robot::CAMERA_X, Robot::CAMERA_Y, 0.0, 0.0)
	{
		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);

		op.SetDriveSquared(true);
		op.SetFlip(OperatorConsole::AnalogControls::DRIVE_X, true);
		op.SetFlip(OperatorConsole::DRIVE_YAW, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_X, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_Y, true);
	}

private:
	void RobotInit() override
	{
		lift.calibrate();
		hold.retract();
	}

	void DisabledInit() override
	{
		compressor.Start();
	}

	void DisabledPeriodic() override
	{
		op.UpdatePreGame();
	}

	void AutonomousInit() override
	{
	}

	void AutonomousPeriodic() override
	{

	}

	void TeleopInit() override
	{
		SmartDashboard::init();
	}

	void TeleopPeriodic() override
	{
		op.UpdateDriveControls();
		drive.Drive(op.GetDriveX(), op.GetDriveY(), op.GetDriveYaw());
		gimbal.setPosition(op.GetCamX(), op.GetCamY());
		SmartDashboard::PutNumber("position", double(op.GetLiftTarget()));

		lift.setTargetState(op.GetLiftTarget());

		if(op.GetHoldExtend())
			hold.extend();
		else if(op.GetHoldRetract())
			hold.retract();
	}

	void TestInit() override
	{
	}

	void TestPeriodic() override
	{

	}
};

START_ROBOT_CLASS(Turing);
