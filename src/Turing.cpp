#include "Turing.h"

using namespace std;

class Turing: public IterativeRobot, public DRR::LogObject<Turing>
{
private:
	enum RobotID {PRIMARY=0, SECONDARY, TEST} id;
	enum AutoMode {NONE=1, PICKUP_BACKUP, FORWARD_PICKUP_BACKUP, PICKUP_TURN_FORWARD, RC_GRAB_BACK, RC_GRAB};
	typedef std::chrono::time_point<std::chrono::system_clock> second_time;
	second_time timer;
	static const second_time getNow() {return std::chrono::system_clock::now();};
	const bool timeExceeds() {return getNow() > timer;};
	void setTimer(const std::chrono::milliseconds s) {timer = getNow() + s;};

private:
	AutoMode autoMode;
	int autoState;
	double fastDrive, slowDrive;

private:
	DRR::MessageConfig config;
	PowerDistributionPanel PDP;
	DalekDrive drive;
	Compressor compressor;
	Lifter lift;
	OperatorConsole op;
	CameraGimbal gimbal;
	Cobra sweep;
	Aligners align;
	Razor razor;

public:
	Turing(): id(PRIMARY), autoMode(NONE), autoState(0), fastDrive(0.4), slowDrive(0.2),//routine(PLATFORM), position(LANDFILL),
			  drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT, 50.0),
			  lift(Robot::LIFT_1, Robot::LIFT_2, Lifter::PIDConfig(2.0, 0.000, 0.0, 20), 50.0),
			  op(Robot::DRIVER_LEFT, Robot::DRIVER_RIGHT, Robot::COPILOT_LEFT, Robot::COPILOT_RIGHT),
			  gimbal(Robot::CAMERA_X, Robot::CAMERA_Y),
			  sweep(Robot::RC_GRABBER, Lifter::PIDConfig(3.0, 0.005, 0.5, 3000.0), Lifter::PIDConfig(11.0, 0.0, 0.0, 0.0), 50.0),
			  align(Robot::ALIGNER_LEFT, Robot::ALIGNER_RIGHT)
	{
		DRR::LogService::LogText("Turing", "main")<<"Constructor started";

		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);

		op.SetDriveSquared(true);
		op.SetFlip(OperatorConsole::AnalogControls::DRIVE_X, true);
		op.SetFlip(OperatorConsole::DRIVE_YAW, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_X, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_Y, false);
		op.SetFlip(OperatorConsole::AnalogControls::LIFT, true);
		op.SetFlip(OperatorConsole::AnalogControls::BIN_PULL, true);

		LogObject<PowerDistributionPanel> pdpLog(&PDP);
		pdpLog.AddLog("total_voltage", &PowerDistributionPanel::GetVoltage, 0);
		pdpLog.AddLog("total_current", &PowerDistributionPanel::GetTotalCurrent, 0);
		pdpLog.AddLog("total_power", &PowerDistributionPanel::GetTotalPower, 0);
		pdpLog.AddLog("total_energy", &PowerDistributionPanel::GetTotalEnergy, 0);

		for(int i = 0; i < 16; i++)
		{
			pdpLog.AddLog<double>(DRR::LogService::AddID("current", i), std::bind(&PowerDistributionPanel::GetCurrent, &PDP, i), 0);
		}

		DRR::LogService::LogText("Turing", "main")<<"Constructor Complete";
	}

private:
	void RobotInit() override
	{
		DRR::LogService::Start();
		//razor.Init();
		lift.calibrate();
		lift.setTargetState(Lifter::Ground);
		gimbal.setState(CameraGimbal::TOTE_VIEW);
	}

	void DisabledInit() override
	{
		align.Retract();
		autoMode = config.GetValue<int>("auto_mode", autoMode);
	}

	void DisabledPeriodic() override
	{
		op.UpdatePreGame();
		int m;
		if(m = op.GetAutoMode())
		{
			LogText()<<"Auto Mode "<<m;
			config.SetValue<int>("auto_mode", m);
		}

		//razor.Update();
		sweep.setMode(Cobra::Position);
	}

	void AutonomousInit() override
	{
		LogText()<<"AutonomousInit started";
		sweep.setMode(Cobra::Position);
		sweep.setState(Cobra::Up);
		autoState = 0;
		autoMode = config.GetValue<int>("auto_mode", autoMode);
	}

	void AutonomousPeriodic() override
	{
		switch(autoMode)
		{
		case NONE:
			None();
			break;
		case PICKUP_BACKUP:
			PickupBackup(false);
			break;
		case FORWARD_PICKUP_BACKUP:
			PickupBackup(true);
			break;
		case PICKUP_TURN_FORWARD:
			PickupTurnForward();
			break;
		case RC_GRAB_BACK:
			GrabRCBack();
			break;
		case RC_GRAB:
			GrabRC();
			break;
		default:
			None();
			break;
		}
	}

	void TeleopInit() override
	{
		sweep.setMode(Cobra::Velocity);
		sweep.setState(Cobra::Hold);
	}

	void TeleopPeriodic() override
	{
		op.UpdateDriveControls();

		drive.Drive(op.GetDriveX(), op.GetDriveY(), op.GetDriveYaw());

		if(op.GetSweeperMode() == Cobra::RawVoltage)
		{
			sweep.setVBus(op.GetBinPull());
		}
		else if(op.GetSweeperState() != Cobra::Transition)
		{
			sweep.setState(op.GetSweeperState());
		}
		else
		{
			sweep.setVelocity(op.GetBinPull());
		}

		if(fabs(op.GetLift()) > 0.01)
		{
			lift.offsetTarget(op.GetLift());
		}
		else if(op.GetGround())
		{
			lift.setTargetState(Lifter::Ground);
		}
		else if(op.GetChute())
		{
			lift.setTargetState(Lifter::Chute);
		}
		//razor.Update();
	}

	void TestInit() override
	{
	}

	void TestPeriodic() override
	{

	}

	void None()
	{
		return;
	}

	void PickupBackup(bool lilForward)
	{
		switch(autoState)
		{
		case 0:	// Drive forward slightly
			if(!lilForward)
			{
				setTimer(std::chrono::milliseconds(0));
				autoState++;
				break;
			}
			drive.Drive(0.0, -slowDrive, 0.0);
			setTimer(std::chrono::milliseconds(500));
			autoState++;
			break;
		case 1:
			if(timeExceeds())
			{
				drive.Drive(0.0, 0.0, 0.0);
				lift.offsetTarget(10.0);
				lift.setTargetState(Lifter::BinT1);
				setTimer(std::chrono::milliseconds(1800));
				autoState++;
			}
			break;
		case 2:
			if(lift.getCurrentState() == Lifter::BinT1 || timeExceeds())
			{
				lift.setTargetState(Lifter::BinT1);
				autoState++;
			}
			break;
		case 3:
			drive.Drive(0.0, fastDrive, 0.0);
			setTimer(std::chrono::milliseconds(2000));
			autoState++;
			break;
		case 4:
			if(timeExceeds())
			{
				drive.Drive(0.0, 0.0, 0.0);
			}
			break;
		}
	}

	void PickupTurnForward()
	{
		switch(autoState)
		{
		case 0:
			lift.setTargetState(Lifter::Chute);
			if(lift.getCurrentState() == Lifter::Chute)
				autoState++;
			break;
		case 1:
			drive.Drive(0.0, 0.0, slowDrive);
			setTimer(std::chrono::milliseconds(750));
			autoState++;
			break;
		case 2:
			if(timeExceeds())
			{
				drive.Drive(0.0, -fastDrive, 0.0);
				autoState++;
				setTimer(std::chrono::milliseconds(2000));
			}
			break;
		case 3:
			if(timeExceeds())
			{
				drive.Drive(0.0, 0.0, 0.0);
				autoState++;
			}
			break;
		default:
			drive.Drive(0.0, 0.0, 0.0);
			break;
		}
	}

	void GrabRCBack()
	{
		switch(autoState)
		{
		case 0:
			sweep.setState(Cobra::Down);
			setTimer(std::chrono::milliseconds(2500));
			LogText()<<"RC Down";
			autoState++;
			break;
		case 1:
			if(timeExceeds() || sweep.getCurrentState() == Cobra::Down)
			{
				drive.Drive(0.0, 0.0, 0.0);
				setTimer(std::chrono::milliseconds(500));
				autoState++;
			}
			break;
		case 2:
			if(timeExceeds())
			{
				sweep.setState(Cobra::Up);
				setTimer(std::chrono::milliseconds(5000));
				autoState++;
			}
			break;
		case 3:
			if(timeExceeds())
			{
				drive.Drive(0.0, -fastDrive, 0.0);
				setTimer(std::chrono::milliseconds(1500));
				autoState++;
			}
			break;
		case 4:
			if(timeExceeds())
			{
				drive.Drive(0.0, 0.0, 0.0);
			}
			break;
		default:
			drive.Drive(0.0, 0.0, 0.0);
			break;
		}
}


	void GrabRC()
	{
		switch(autoState)
		{
		case 0:
			sweep.setState(Cobra::Down);
			setTimer(std::chrono::milliseconds(2500));
			LogText()<<"RC Down";
			autoState++;
			break;
		case 1:
			if(timeExceeds() || sweep.getCurrentState() == Cobra::Down)
			{
				drive.Drive(0.0, slowDrive, 0.0);
				setTimer(std::chrono::milliseconds(500));
				autoState++;
			}
			break;
		case 2:
			if(timeExceeds())
			{
				drive.Drive(0.0, 0.0, 0.0);
				setTimer(std::chrono::milliseconds(500));
				autoState++;
			}
			break;
		case 3:
			if(timeExceeds())
			{
				sweep.setState(Cobra::Up);
				setTimer(std::chrono::milliseconds(5000));
				autoState++;
			}
			break;
		case 4:
			if(timeExceeds())
			{
				drive.Drive(0.0, -fastDrive, 0.0);
				setTimer(std::chrono::milliseconds(1500));
				autoState++;
			}
			break;
		case 5:
			if(timeExceeds())
			{
				drive.Drive(0.0, 0.0, 0.0);
			}
			break;
		default:
			drive.Drive(0.0, 0.0, 0.0);
			break;
		}
	}

};

START_ROBOT_CLASS(Turing);
