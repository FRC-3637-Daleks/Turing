#include "Turing.h"

using namespace std;

class Turing: public IterativeRobot
{
private:
	PowerDistributionPanel PDP;

public:
	Turing()
	{
		Logger::MakeLogValue("TIME", "BOOT_TICKS", &GetTime);
		Logger::MakeLogValue("VOLTAGE", &PDP, &PowerDistributionPanel::GetVoltage);
		Logger::LogState("GENERAL", LEVEL_t::DEBUG, "Turing object constructed");
		cout<<"Built: "<<__DATE__<<' '<<__TIME__<<endl;
	}

private:
	void RobotInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::DEBUG, "Robot init complete");
	}

	void DisabledInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::DEBUG, "Disabled Init Complete");
	}

	void DisabledPeriodic() override
	{
	}

	void AutonomousInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::DEBUG, "Autonomous Init Complete, ready to crack enigma.");
	}

	void AutonomousPeriodic() override
	{

	}

	void TeleopInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::DEBUG, "Teleop Init Complete");
	}

	void TeleopPeriodic() override
	{

	}

	void TestInit() override
	{
		Logger::LogState("GENERAL", LEVEL_t::DEBUG, "Test Init Complete");
	}

	void TestPeriodic() override
	{

	}
};

START_ROBOT_CLASS(Turing);
