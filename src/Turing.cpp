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
	Joystick left, right;

public:
	Turing(): drive(DalekDrive::Wheel_t::MECANUM_WHEELS, 2, 1, 4, 3), left(0), right(1)
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


		Logger::LogState("GENERAL", LEVEL_t::INFO, "Turing object constructed");
		Logger::LogState("GENERAL", LEVEL_t::NOTICE, string("Built: ")+__DATE__+' '+__TIME__);
	}

private:
	void RobotInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::INFO, "Robot init complete");
	}

	void DisabledInit() override
	{
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
