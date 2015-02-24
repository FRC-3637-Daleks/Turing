
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "WPILib.h"

#define MAX_TALON_CURRENT 100

class Lifter {
public:
	enum Height_t {Ground = 0, Step, StackDown, StackUp, ToteScore, ToteDown, ToteUp, BinT1, BinT2, BinT3, Top, NUM_STATES};
	static const double States[];
	static constexpr double ticksPerInch = 183.0;
	static constexpr double inchesOffGround = 3.0;
	static constexpr double toleranceTicks = 20.0;

public:
	struct PIDConfig
	{
		double P;
		double I;
		double D;
		unsigned int iZone;
		PIDConfig(const double iP, const double iI=0.0, const double iD=0.0, const unsigned int iiZone=200): P(iP), I(iI), D(iD), iZone(iiZone) {};
	};

private:
	CANTalon m_tal1, m_tal2;
	PIDConfig pid;
	double rampRate;
	double targetPosition;
	Height_t targetState, previousState;
	bool calibrated;

public:
	Lifter(int talID1, int talID2, PIDConfig iPID, double ramp);
	bool setTargetPosition(double position);
	double getTargetPosition();
	double getCurrentPosition();
	double getDistanceToTarget();
	bool setTargetState(Height_t height);
	Height_t getTargetState();
	Height_t getCurrentState();
	void calibrate();
	const bool isCalibrated() const {return calibrated;};
	void offsetTarget(double offset);
	double inchToTicks(double inches);
	double inchesOffGroundToTicks(double inches);
	void check();
};



#endif /* SRC_LIFTER_H_ */
