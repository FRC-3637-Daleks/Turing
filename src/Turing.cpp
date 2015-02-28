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
	LiftManager manager;
	OperatorConsole op;
	CameraGimbal gimbal;

public:
	Turing(): drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT, 50.0),
			  lift(Robot::LIFT_1, Robot::LIFT_2, Lifter::PIDConfig(2.0, 0.000, 0.0, 20), 50.0),
			  hold(Robot::HOLDER_RETRACT, Robot::HOLDER_EXTEND, Robot::TOTE_SWITCH),
			  manager(lift, hold),
			  op(Robot::DRIVER_LEFT, Robot::DRIVER_RIGHT, Robot::COPILOT_LEFT, Robot::COPILOT_RIGHT),
			  gimbal(Robot::CAMERA_X, Robot::CAMERA_Y, 0.0, 0.0)
	{
		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);

		op.SetDriveSquared(true);
		op.SetFlip(OperatorConsole::AnalogControls::DRIVE_X, true);
		op.SetFlip(OperatorConsole::DRIVE_YAW, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_X, false);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_Y, true);
		op.SetFlip(OperatorConsole::AnalogControls::LIFT, true);
	}

private:
	void RobotInit() override
	{
		lift.calibrate();
		lift.setTargetState(Lifter::Ground);
		manager.EnableManual(false);
		compressor.Start();
	}

	void DisabledInit() override
	{
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

		std::cout<<Lifter::GetName(lift.getCurrentState())<<", "<<Holder::GetName(hold.getCurrentPosition())<<std::endl;
		if(!op.GetManual())
		{
			manager.EnableManual(false);

			if(op.GetGround())
				manager.GoToGround();
			else if(op.GetScoreStep())
				manager.ScoreStackToStep();
			else if(op.GetScore())
				manager.ScoreStack();
			else if(op.GetPushTote())
				manager.PushToteToStack();

			manager.ExecuteCurrent();
		}
		else
		{
			ManualMode();
		}

	}

	void ManualMode()
	{
		if(op.GetIncrementManual())
		{
			manager.EnableManual(false);
			manager.SetHeightTarget(op.GetLiftTarget());
			if(op.GetHoldExtend())
				manager.SetHolderTarget(Holder::HOLDER_OUT);
			else if(op.GetHoldRetract())
				manager.SetHolderTarget(Holder::HOLDER_IN);
		}
		else
		{
			manager.EnableManual(true);
			manager.OffsetTarget(op.GetLift());
			if(op.GetHoldExtend())
				hold.setTargetPosition(Holder::HOLDER_OUT);
			else if(op.GetHoldRetract())
				hold.setTargetPosition(Holder::HOLDER_IN);
		}
	}

	void TestInit() override
	{
	}

	void TestPeriodic() override
	{
		op.UpdateDriveControls();
		drive.Drive(op.GetDriveX(), op.GetDriveY(), op.GetDriveYaw());
		gimbal.setPosition(op.GetCamX(), op.GetCamY());
		ManualMode();
	}
};

START_ROBOT_CLASS(Turing);
