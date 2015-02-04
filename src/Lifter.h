
#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "Holder.h"
#include "WPILib.h"

class Height_t {
public:
	enum {Ground = 0, Tote1, ToteCan, Tote2, Tote3};
	static const double States[];
};

class Lifter {
private:
	CANTalon m_tal1, m_tal2;
	double m_upLim, m_lowLim, m_targetPos;
	double m_P, m_I, m_D;
	Holder m_h;
	Height_t m_targetState;
public:
	Lifter(CANTalon tal1, CANTalon tal2, double P, double I, double D, double upLim, double lowLim, Holder h);
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
