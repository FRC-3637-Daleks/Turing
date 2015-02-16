/*
 * LiftManager.h
 *
 *  Created on: Feb 15, 2015
 *      Author: edward
 */

#ifndef SRC_LIFTMANAGER_H_
#define SRC_LIFTMANAGER_H_

#include "Holder.h"
#include "Lifter.h"

class LiftManager
{
public:
	struct DuelState
	{
		Lifter::Height_t lifterState;
		Holder::holder_t holderState;
	};

private:
	typedef bool (LiftManager::* STATE_FUNC)();
	static const STATE_FUNC funcs[Lifter::NUM_STATES][Holder::NUM_STATES];

private:
	Lifter &lifter;
	Holder &holder;
	DuelState targetState, currentState;
	bool manual;

public:
	LiftManager(Lifter &lift, Holder &hold): lifter(lift), holder(hold), manual(false) {};
	virtual ~LiftManager() {};

public:
	const Lifter::Height_t GetCurrentHeight() {return lifter.getCurrentState();};
	const Holder::holder_t GetCurrentHoldState() {return holder.getPosition();};

private:
	const DuelState ResolveCurrentState() {return {lifter.getCurrentState(), holder.getPosition()};};
	const bool ExecuteCurrent();

public:	/// State functions
	bool MoveHook();
	bool MoveHookOrExtend();
	bool ResolveHolder();
	bool MoveHookOrRetract();
	bool PushToteToStack();
	bool LiftStack();
	bool Death();
};



#endif /* SRC_LIFTMANAGER_H_ */
