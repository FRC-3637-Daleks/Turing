#include "Turing.h"

using namespace std;

Loggable::FAIL_COMMAND currentTooHigh(double cur)
{
	stringstream curString;
	curString<<cur;
	Logger::LogState("POWERDISTRIBUTIONPANEL", LEVEL_t::ALERT, string("Current level too high: ")+curString.str()+" amps");
	return Loggable::KILL;
}

class Turing: public IterativeRobot
{
private:
	PowerDistributionPanel PDP;
	DalekDrive drive;
	Lifter lift;
	Holder hold;
	Joystick left, right;
	Compressor compressor;

public:
	Turing(): drive(DalekDrive::Wheel_t::MECANUM_WHEELS, Robot::FRONT_LEFT, Robot::FRONT_RIGHT, Robot::BACK_LEFT, Robot::BACK_RIGHT),
			  lift(5, 6, 2.0, 0.005, 0.0, 20, 50.0, 11550., -1.0), hold(0, 1), left(0), right(1)
	{
		drive[DalekDrive::LEFT_FRONT].SetFlip(true);
		drive[DalekDrive::LEFT_REAR].SetFlip(true);
		/*TODO: Have Dash Thread poll Log thread for most recent data
		 * Write function in ValueLog which returns most previous val
		 */
		for(int i = 0; i <= 15; i++)
		{
			auto f = Logger::MakeLogValue<double>("PowerDistributionPanel", Logger::MakeComponentName("current", i).c_str(), std::bind(&PowerDistributionPanel::GetCurrent, &PDP, i));
			SmartDashService::GetInstance().addLog<double>(f, Logger::MakeComponentName("pdp_current", i));
		}

		auto volt = Logger::MakeLogValue("VOLTAGE", &PDP, &PowerDistributionPanel::GetVoltage);
		SmartDashService::GetInstance().addLog<double>(volt, "pdp_voltage");

		auto compressorCur = Logger::MakeLogValue("compressor_current", &compressor, &Compressor::GetCompressorCurrent);
		SmartDashService::GetInstance().addLog(compressorCur, "compressor_current");

		Logger::StartLogging();
		SmartDashService::GetInstance().startLogging();

		Logger::LogState("GENERAL", LEVEL_t::INFO, "Turing object constructed");
		Logger::LogState("GENERAL", LEVEL_t::NOTICE, string("Built: ")+__DATE__+' '+__TIME__);
	}

private:
	void RobotInit() override
	{
		lift.calibrate();
		hold.setPosition(Holder::HOLDER_IN);
		Logger::LogState("GENERAL", LEVEL_t::INFO, "Robot init complete");
	}

	void DisabledInit() override
	{
		compressor.Start();
		Logger::LogState("GENERAL", LEVEL_t::INFO, "Disabled Init Complete");
	}

	void DisabledPeriodic() override
	{
	}

	void AutonomousInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::INFO, "Autonomous Init Complete, ready to crack enigma.");
	}

	void AutonomousPeriodic() override
	{

	}

	void TeleopInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::INFO, "Teleop Init Complete");
	}

	void TeleopPeriodic() override
	{
		drive.Drive(right.GetX(), right.GetY(), left.GetX());
		if(left.GetRawButton(1))
		{
			lift.setTargetPosition(0.0);
		}
		else if(left.GetRawButton(2))
		{
			lift.setTargetPosition(1000.0);
		}
		else if(left.GetRawButton(3))
		{
			lift.setTargetPosition(6000.0);
		}

		if(right.GetRawButton(3))
		{
			hold.setPosition(Holder::HOLDER_OUT);
		}
		else if(right.GetRawButton(2))
		{
			hold.setPosition(Holder::HOLDER_IN);
		}
	}

	void TestInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::INFO, "Test Init Complete");
	}

	void TestPeriodic() override
	{

	}
};

START_ROBOT_CLASS(Turing);
