
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "WPILib.h"

class Lifter {
public:
	/// Enumeration of predefined heights the Lifter can be in. Measured in inches.
	enum Height_t {Ground = 0, Step, StackDown, StackUp, ToteDown, ToteUp, BinT1, BinT2, BinT3, Top, NUM_STATES};
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
	/// Constructor accepts IDs of 2 talons, PID values, and a Ramp Rate. Talon 2 is slaved to talon 1, and Lifter is calibrated.
	Lifter(int talID1, int talID2, PIDConfig iPID, double ramp);

	/// Calibrate() is meant to put the Lifter at the Ground state and reset the position to 0.
	void calibrate();

	const bool isCalibrated() const {return calibrated;};

	/// Accepts a target position and sets Talons to that position. Returns true once Lifter reaches position.
	bool setTargetPosition(double position);

	/// Returns position that the Lifter is going to..
	double getTargetPosition();

	/// Returns the current position the Lifter is at.
	double getCurrentPosition();

	/// Returns the distance, in encoder ticks, to the target position
	double getDistanceToTarget();

	/// Sets target position to a predefined height/state.
	bool setTargetState(Height_t height);

	/// Returns target state that the Lifter is going to.
	Height_t getTargetState();

	/// If the lifter is within a certain bound, it returns the next state it is going towards. Otherwise it returns the previous state.
	Height_t getCurrentState();

	/// Offsets current position by the input amount
	void offsetTarget(double offset);

	/// Conversion function that converts inches to encoder ticks
	double inchToTicks(double inches);

	/// Subtracts distance from the bottom of the Lifter to the ground
	double inchesOffGroundToTicks(double inches);

	/// If the output current of the talons spike, offset the target position by 100 encoder ticks to avoid overshooting.
	void check();
};



#endif /* SRC_LIFTER_H_ */
