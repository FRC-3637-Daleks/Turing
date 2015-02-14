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
	Turing(): drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT),
			  lift(5, 6, 2.0, 0.005, 0.0, 20, 50.0, 11550, -1.0, 155, 3.0), hold(0, 1), op(0, 1, 2, 3), gimbal(0, 1, 0.0, 0.0)
	{
		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);

		op.SetDriveSquared(true);
		op.SetFlip(OperatorConsole::AnalogControls::DRIVE_X, true);
		op.SetFlip(OperatorConsole::DRIVE_YAW, true);
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
	}

	void TeleopPeriodic() override
	{
		op.UpdateDriveControls();
		drive.Drive(op.GetDriveX(), op.GetDriveY(), op.GetDriveYaw());
		gimbal.setPosition(op.GetCamX(), op.GetCamY());
		//lift.setTargetState(op.GetLiftTarget());
	}

	void TestInit() override
	{
	}

	void TestPeriodic() override
	{

	}
};

START_ROBOT_CLASS(Turing);
