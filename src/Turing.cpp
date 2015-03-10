#include "Turing.h"

using namespace std;

class Turing: public IterativeRobot
{
private:
	enum RobotID {PRIMARY=0, SECONDARY, TEST} id;
	enum AutoMode_t {EARLY, UNDER, HOLDING, DRIVING, PLACING, BACKING, END} mode;
	//enum AutoRoutine_t {PLATFORM, NON_PLATFORM} routine;
	//enum AutoStart_t {LANDFILL, CONTAINER} position;
	typedef std::chrono::time_point<std::chrono::system_clock> second_time;
	second_time timer;
	static const second_time getNow() {return std::chrono::system_clock::now();};
	const bool timeExceeds() {return getNow() > timer;};
	void setTimer(const std::chrono::seconds s) {timer = getNow() + s;};

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
	Turing(): id(PRIMARY), mode(EARLY), //routine(PLATFORM), position(LANDFILL),
			  drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT, 50.0),
			  lift(Robot::LIFT_1, Robot::LIFT_2, Lifter::PIDConfig(2.0, 0.000, 0.0, 20), 50.0),
			  op(Robot::DRIVER_LEFT, Robot::DRIVER_RIGHT, Robot::COPILOT_LEFT, Robot::COPILOT_RIGHT),
			  gimbal(Robot::CAMERA_X, Robot::CAMERA_Y),
			  sweep(Robot::RC_GRABBER, Lifter::PIDConfig(10.0, 0.0, 0.0, 20.0), 45.0), align(Robot::ALIGNER_LEFT, Robot::ALIGNER_RIGHT)
	{
		RobotConf idFile("robotID.conf");
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

		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);

		op.SetDriveSquared(true);
		op.SetFlip(OperatorConsole::AnalogControls::DRIVE_X, true);
		op.SetFlip(OperatorConsole::DRIVE_YAW, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_X, true);
		op.SetFlip(OperatorConsole::AnalogControls::CAM_Y, false);
		op.SetFlip(OperatorConsole::AnalogControls::LIFT, true);
		DRR::LogService::LogText("Turing")<<"Constructor Complete";
	}

private:
	void RobotInit() override
	{
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
		mode = EARLY;
		//routine = SmartDashboard::GetBoolean("auto_long", true)? PLATFORM:NON_PLATFORM;
		//position = SmartDashboard::GetBoolean("auto_landfill", true)? LANDFILL:CONTAINER;
		//std::cout<<"routine: "<<(routine==PLATFORM? "PLATFORM":"NON_PLATFORM")<<", "<<(position==LANDFILL? "LANDFILL":"CONTAINER")<<std::endl;
	}

	void AutonomousPeriodic() override
	{
		switch(mode)
		{
		case EARLY:
			/*if(position == PLATFORM)
			{
				drive.Drive(0.0, -0.14, 0.0);
				setTimer(std::chrono::seconds(1));
			}
			else
			{*/
			drive.Drive(0.0, -0.14, 0.0);
			setTimer(std::chrono::seconds(1));
			mode = UNDER;
			break;
		case UNDER:
			if(!timeExceeds())
				break;
			drive.Drive(0.0,0.0,0.0);
			break;
		case HOLDING:
			drive.Drive(0.0, 0.12, 0.0);
			setTimer(std::chrono::seconds(10));
			mode = DRIVING;
			break;
		case DRIVING:
			/*if(position == PLATFORM)
				drive.Drive(0.0, -0.23, 0.0);
				*/
			if(timeExceeds())
				mode = PLACING;
			break;
		case PLACING:
			drive.Drive(0.0, 0.12, 0.0);
			/*manager.SetHeightTarget(Lifter::Ground);
			if(manager.GetCurrentHeight() == Lifter::Ground)*/
				mode = BACKING;
			break;
		case BACKING:
			drive.Drive(0.0, 0.0, 0.0);
			setTimer(std::chrono::seconds(1));
			mode = END;
			break;
		case END:
			if(timeExceeds())
				drive.Drive(0.0, 0.0, 0.0);
			break;
		}
	}

	void TeleopInit() override
	{
	}

	void TeleopPeriodic() override
	{
		op.UpdateDriveControls();

		drive.Drive(op.GetDriveX(), op.GetDriveY(), op.GetDriveYaw());

		if(op.GetAlignExtend())
			align.Extend();
		else
			align.Retract();

		sweep.offset(op.GetBinPull());

		if(op.GetManual())
			lift.offsetTarget(op.GetLift());
	}

	void TestInit() override
	{
	}

	void TestPeriodic() override
	{

	}

};

START_ROBOT_CLASS(Turing);
