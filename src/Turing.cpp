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
