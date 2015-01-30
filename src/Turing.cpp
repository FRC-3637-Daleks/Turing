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

public:
	Turing()
	{
		/*TODO: Have Dash Thread poll Log thread for most recent data
		 * Write function in ValueLog which returns most previous val
		 */
		for(int i = 0; i <= 15; i++)
		{
			stringstream name;
			name<<"pdp_current_"<<i;
			auto f = Logger::MakeLogValue<double>("PowerDistributionPanel", name.str().c_str(), std::bind(&PowerDistributionPanel::GetCurrent, &PDP, i));
			SmartDashService::GetInstance().addLog<double>(f, name.str());
		}


		auto volt = Logger::MakeLogValue("VOLTAGE", &PDP, &PowerDistributionPanel::GetVoltage);
		/*auto cur = Logger::MakeLogValue("TOTALCURRENT", &PDP, &PowerDistributionPanel::GetTotalCurrent,
				MakeWatchLog<double>(currentTooHigh, [](double cur) {return cur > 100.0;}));
		*/
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
