
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "WPILib.h"

class Lifter {
public:
	enum Height_t {Ground = 0, Tote1, ToteCan, Tote2, Tote3};
	static const double States[];
private:
	CANTalon m_tal1, m_tal2;
	double m_upLim, m_lowLim, m_targetPos;
	double m_P, m_I, m_D;
	Height_t m_targetState;
public:
	Lifter(int talID1, int talID2, double P, double I, double D,
			int iZone, double rampRate, double upLim, double lowLim);
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
};



#endif /* SRC_LIFTER_H_ */
