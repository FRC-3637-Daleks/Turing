/*
 * LiftManager.h
 *
 *  Created on: Feb 15, 2015
 *      Author: edward
 */

#ifndef SRC_LIFTMANAGER_H_
#define SRC_LIFTMANAGER_H_

#include <queue>

#include "Holder.h"
#include "Lifter.h"

using std::queue;

class LiftManager
{
public:
	struct DuelState
	{
		Lifter::Height_t lifterState;
		Holder::holder_t holderState;
		DuelState(const int l, const int h): lifterState(l), holderState(h) {};
	};

	typedef queue<DuelState> Routine;

private:
	typedef bool (LiftManager::* STATE_FUNC)();
	static const STATE_FUNC funcs[Lifter::NUM_STATES][Holder::NUM_STATES];

private:
	Lifter &lifter;
	Holder &holder;
	DuelState targetState, currentState;
	Routine *currentRoutine;

public:
	LiftManager(Lifter &lift, Holder &hold): lifter(lift), holder(hold), currentRoutine(NULL) {};
	virtual ~LiftManager() {};

public:
	const Lifter::Height_t GetCurrentHeight() {return lifter.getCurrentState();};
	const Holder::holder_t GetCurrentHoldState() {return holder.getPosition();};
	void SetHeightTarget(const Lifter::Height_t h);
	void SetHolderTarget(const Holder::holder_t h);
	void OffsetTarget(const double inches);
	void CancelRoutine();
	const bool GoToGround();
	const bool PushToteToStack();
	const bool ScoreStack();
	const bool ScoreStackToStep();

public:
	const DuelState GetCurrentState() const {return currentState;};
	const DuelState GetTargetState() const {return targetState;};

private:
	const DuelState ResolveCurrentState() {return {lifter.getCurrentState(), holder.getPosition()};};
	void GoToState(const DuelState &state);
	const bool ExecuteCurrent();

private:	/// State functions
	bool MoveHook();
	bool MoveHookOrExtend();
	bool ResolveHolder();
	bool MoveHookOrRetract();
	bool Safety();
	bool LiftStack();
	bool Death();
};



#endif /* SRC_LIFTMANAGER_H_ */
