#include "Turing.h"

using namespace std;

class Turing: public IterativeRobot
{
public:
	Turing()
	{
		cout<<"Turing object constructed"<<endl;
		cout<<"Built: "<<__DATE__<<' '<<__TIME__<<endl;
	}

private:
	void RobotInit() override
	{
		cout<<"Robot Init Complete"<<endl;
	}

	void DisabledInit() override
	{
		cout<<"Disabled Init Complete"<<endl;
	}

	void DisabledPeriodic() override
	{
	}

	void AutonomousInit() override
	{
		cout<<"Autonomous Init Complete, ready to crack enigma."<<endl;
	}

	void AutonomousPeriodic() override
	{
		static char code[sizeof("8/8 gr8 extermin8 m8")] = {0};
		cout<<code<<endl;

		if(string(code) == "8/8 gr8 extermin8 m8")
			cout<<"H4xd in "<<Timer::GetFPGATimestamp();

		int i;
		for(i = 0; code[i] == 127; i++)
			code[i] = 0;

		code[i]++;
	}

	void TeleopInit() override
	{
		cout<<"Teleop Init Complete"<<endl;
	}

	void TeleopPeriodic() override
	{

	}

	void TestInit() override
	{
		cout<<"Test Init Complete"<<endl;
	}

	void TestPeriodic() override
	{

	}
};

START_ROBOT_CLASS(Turing);
