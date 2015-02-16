/*
 * LiftManager.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: edward
 */

#include "LiftManager.h"

const LiftManager::STATE_FUNC LiftManager::funcs[][Holder::NUM_STATES] = {
	[Lifter::Ground] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHook,
			[Holder::HOLDER_OUT] = &LiftManager::ResolveHolder,
			[Holder::HOLDING] = &LiftManager::MoveHook
	},

	[Lifter::Step] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHook,
			[Holder::HOLDER_OUT] = &LiftManager::ResolveHolder,
			[Holder::HOLDING] = &LiftManager::MoveHook
	},

	[Lifter::StackDown] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHook,
			[Holder::HOLDER_OUT] = &LiftManager::ResolveHolder,
			[Holder::HOLDING] = &LiftManager::MoveHook
	},

	[Lifter::StackUp] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHook,
			[Holder::HOLDER_OUT] = &LiftManager::Safety,
			[Holder::HOLDING] = &LiftManager::LiftStack
	},

	[Lifter::ToteDown] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHook,
			[Holder::HOLDER_OUT] = &LiftManager::ResolveHolder,
			[Holder::HOLDING] = &LiftManager::MoveHook
	},

	[Lifter::ToteUp] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHookOrExtend,
			[Holder::HOLDER_OUT] = &LiftManager::MoveHookOrRetract,
			[Holder::HOLDING] = &LiftManager::LiftStack
	},

	[Lifter::BinT1] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHookOrExtend,
			[Holder::HOLDER_OUT] = &LiftManager::MoveHookOrRetract,
			[Holder::HOLDING] = &LiftManager::Death
	},

	[Lifter::BinT2] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHookOrExtend,
			[Holder::HOLDER_OUT] = &LiftManager::MoveHookOrRetract,
			[Holder::HOLDING] = &LiftManager::Death
	},

	[Lifter::BinT3] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHookOrExtend,
			[Holder::HOLDER_OUT] = &LiftManager::MoveHookOrRetract,
			[Holder::HOLDING] = &LiftManager::Death
	},

	[Lifter::Top] = {
			[Holder::HOLDER_IN] = &LiftManager::MoveHookOrExtend,
			[Holder::HOLDER_OUT] = &LiftManager::MoveHookOrRetract,
			[Holder::HOLDING] = &LiftManager::Death
	}
};

const bool LiftManager::ExecuteCurrent()
{
	currentState = ResolveCurrentState();

	bool ret;
	if(funcs[targetState.lifterState][targetState.holderState] == NULL)
		ret = false;
	else
		ret = (this->*(funcs[targetState.lifterState][targetState.holderState]))();

	if(ret && currentRoutine != NULL)
	{
		if(currentState == targetState)	// Next state in routine
		{
			if(currentRoutine->front().holderState >= Holder::HOLDER_IN)
				targetState.holderState = currentRoutine->front().holderState;

			if(currentRoutine->front().lifterState >= Lifter::Ground)
				targetState.lifterState = currentRoutine->front().lifterState;

			currentRoutine->pop();
			if(currentRoutine->empty())
			{
				delete currentRoutine;
				currentRoutine = NULL;
			}
		}
	}

	return ret;
}

void LiftManager::GoToState(const DuelState &state)
{
	holder.setPosition(targetState.holderState);

	// Allows manual movement
	if(currentState.lifterState != targetState.lifterState)
		lifter.setTargetState(targetState.lifterState);

}

void LiftManager::SetHeightTarget(const Lifter::Height_t h)
{
	targetState.lifterState = h;
	ExecuteCurrent();
}

void LiftManager::SetHolderTarget(const Holder::holder_t h)
{
	targetState.holderState = h;
	ExecuteCurrent();
}

void LiftManager::OffsetTarget(const double inches)
{
	lifter.offsetTarget(inches);
	ExecuteCurrent();
}

void LiftManager::CancelRoutine()
{
	if(currentRoutine != NULL)
	{
		delete currentRoutine;
		currentRoutine = NULL;
	}
}

const bool LiftManager::GoToGround()
{
	CancelRoutine();
	SetHeightTarget(Lifter::Ground);
	return true;
}

const bool LiftManager::PushToteToStack()
{
	CancelRoutine();
	if(currentState.lifterState != Lifter::Ground)
		return false;
	currentRoutine = new queue<DuelState>;
	currentRoutine->push(DuelState(Lifter::ToteUp, -1));
	currentRoutine->push(DuelState(-1, Holder::HOLDER_OUT));
	currentRoutine->push(DuelState(Lifter::ToteDown, -1));

	return true;
}

const bool LiftManager::ScoreStackToStep()
{
	CancelRoutine();
	currentRoutine = new queue<DuelState>;
	if(currentState.holderState == Holder::HOLDING)
		currentRoutine->push(DuelState(Lifter::ToteUp, -1));

	currentRoutine->push(DuelState(-1, Holder::HOLDER_IN));
	currentRoutine->push(DuelState(Lifter::Step, -1));

	return true;
}

const bool LiftManager::ScoreStack()
{
	ScoreStackToStep();
	currentRoutine->push(DuelState(Lifter::Ground, -1));
}


bool LiftManager::MoveHook()
{
	// Disallow hook movement in these areas
	if(targetState.holderState == Holder::HOLDER_OUT)
		targetState.holderState = currentState.holderState;

	// Make sure to move to the correct position, not surpassing StackUp on the wrong side
	if(targetState.holderState != Holder::HOLDER_IN && targetState.lifterState >= Lifter::StackUp)
		GoToState(DuelState(Lifter::StackUp, targetState.holderState));
	else
		GoToState(targetState);
	return true;
}

bool LiftManager::MoveHookOrExtend()
{
	GoToState(targetState);
	return true;
}

bool LiftManager::ResolveHolder()
{
	if(holder.getPosition() == Holder::HOLDING)
	{
		targetState.holderState = Holder::HOLDING;
	}
	else if(holder.getPosition() == Holder::HOLDER_OUT)
	{
		targetState.holderState = Holder::HOLDER_IN;
		holder.retract();
	}

	GoToState(targetState);
	return true;
}

bool LiftManager::MoveHookOrRetract()
{
	if(targetState.holderState == Holder::HOLDING)
		targetState.holderState = Holder::HOLDER_OUT;
	GoToState(targetState);
	return true;
}

bool LiftManager::Safety()
{
	// Ensures it does not break
	if(targetState.lifterState > Lifter::StackUp)
	{
		targetState.holderState = Holder::HOLDER_IN;
	}

	// Ensures target is accurate
	if(targetState.holderState == Holder::HOLDING)
		targetState.holderState = Holder::HOLDER_OUT;

	GoToState(targetState);
	return true;
}

bool LiftManager::LiftStack()
{
	GoToState(targetState);
	return true;
}

bool LiftManager::Death()
{
	// It's over
	return 0/0;
}
