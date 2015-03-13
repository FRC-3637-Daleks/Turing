#include "Turing.h"

using namespace std;

class Turing: public IterativeRobot
{
private:
	enum RobotID {PRIMARY=0, SECONDARY, TEST} id;
	enum AutoMode {NONE, PICKUP_BACKUP};
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
	RobotConf config;
	PowerDistributionPanel PDP;
	DalekDrive drive;
	Compressor compressor;
	Lifter lift;
	OperatorConsole op;
	CameraGimbal gimbal;
	Sweeper sweep;
	Aligners align;

public:
	Turing(): id(PRIMARY), autoState(0), fastDrive(0.5), slowDrive(0.2),//routine(PLATFORM), position(LANDFILL),
			  drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT, 50.0),
			  lift(Robot::LIFT_1, Robot::LIFT_2, Lifter::PIDConfig(2.0, 0.000, 0.0, 20), 50.0),
			  op(Robot::DRIVER_LEFT, Robot::DRIVER_RIGHT, Robot::COPILOT_LEFT, Robot::COPILOT_RIGHT),
			  gimbal(Robot::CAMERA_X, Robot::CAMERA_Y),
			  sweep(Robot::RC_GRABBER, Lifter::PIDConfig(8.0, 0.0, 0.2, 00.0), Lifter::PIDConfig(10.0, 0.0, 0.0, 0.0), 50.0),
			  align(Robot::ALIGNER_LEFT, Robot::ALIGNER_RIGHT)
	{
		DRR::LogService::LogText("Turing")<<"Constructor started";
		/*RobotConf idFile("robotID.conf");
		if(!idFile.HasValue("id"))
		{
			idFile.SetValue<int>("id", PRIMARY);
		}
		id = RobotID(idFile.GetValue<int>("id"));

		if(!config.HasValue("ticks_per_inch"))
		{
			if(id == PRIMARY)
				config.SetValue<double>("ticks_per_inch", 120.0);
			else
				config.SetValue<double>("ticks_per_inch", 180.0);
		}
		Lifter::ticksPerInch = config.GetValue<double>("ticks_per_inch");

		idFile.Save();
		config.Save();
		*/

		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);

		op.SetDriveSquared(true);
		op.SetFlip(OperatorConsole::AnalogControls::DRIVE_X, true);
		op.SetFlip(OperatorConsole::DRIVE_YAW, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_X, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_Y, false);
		op.SetFlip(OperatorConsole::AnalogControls::LIFT, true);
		op.SetFlip(OperatorConsole::AnalogControls::BIN_PULL, true);

		DRR::LogService::LogText("Turing")<<"Constructor Complete";
	}

private:
	void RobotInit() override
	{
		DRR::MosCutie::Subscribe("roborio/config/#");
		DRR::LogService::Start();
		lift.calibrate();
		lift.setTargetState(Lifter::Ground);
		gimbal.setState(CameraGimbal::TOTE_VIEW);
	}

	void DisabledInit() override
	{
		align.Retract();
	}

	void DisabledPeriodic() override
	{
		op.UpdatePreGame();
	}

	void AutonomousInit() override
	{
		autoState = 0;
		auto modeStr = DRR::MosCutie::Get("roborio/config/auto_mode", false);
		if(modeStr == "none")
			autoMode = NONE;
		else if(modeStr == "pickup_backup")
			autoMode = PICKUP_BACKUP;
		else
			autoMode = NONE;
	}

	void AutonomousPeriodic() override
	{
		switch(autoMode)
		{
		case NONE:
			None();
			break;
		case PICKUP_BACKUP:
			PickupBackup();
			break;
		default:
			None();
			break;
		}
	}

	void TeleopInit() override
	{
		sweep.setMode(Sweeper::RawVoltage);
	}

	void TeleopPeriodic() override
	{
		op.UpdateDriveControls();

		drive.Drive(op.GetDriveX(), op.GetDriveY(), op.GetDriveYaw());

		if(op.GetSweeperMode() == Sweeper::RawVoltage)
		{
			sweep.setVBus(op.GetBinPull());
		}
		else if(op.GetSweeperState() != Sweeper::Transition)
		{
			sweep.setState(op.GetSweeperState());
		}
		else
		{
			sweep.setVelocity(op.GetBinPull());
		}

		lift.offsetTarget(op.GetLift());
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

	void PickupBackup()
	{
		switch(autoState)
		{
		case 0:	// Drive forward slightly
			drive.Drive(0.0, -slowDrive, 0.0);
			setTimer(std::chrono::milliseconds(500));
			autoState++;
			break;
		case 1:
			if(timeExceeds())
				lift.setTargetState(Lifter::Chute);
			if(lift.getCurrentState() == Lifter::Chute)
				autoState++;
			break;
		case 2:
			drive.Drive(0.0, fastDrive, 0.0);
			setTimer(std::chrono::milliseconds(2000));
			autoState++;
			break;
		case 3:
			drive.Drive(0.0, 0.0, 0.0);
			break;
		}
	}

};

START_ROBOT_CLASS(Turing);
