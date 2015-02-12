
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "WPILib.h"

class Lifter {
public:
	enum Height_t {Ground = 0, Step, StickUp, Bin0, Bin1, BinUp, Holding, ToteUp, BinT1};
	static const double States[];
private:
	CANTalon m_tal1, m_tal2;
	int m_iZone;
	double m_upLim, m_lowLim, m_targetPos, m_rampRate;
	double m_P, m_I, m_D;
	Height_t m_targetState;
	bool calibrated;
	double m_ticksPerInch, m_inchesOffGround;

public:
	Lifter(int talID1, int talID2, double P, double I, double D,
			int iZone, double rampRate, double upLim, double lowLim,
			double ticksPerInch, double inchesOffGround);
	void setP(double P);
	double getP();
	void setI(double I);
	double getI();
	void setD(double D);
	double getD();
	void setUpLim(double upLim);
	double getUpLim();
	void setLowLim(double lowLim);
	double getLowLim();
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
};



#endif /* SRC_LIFTER_H_ */
