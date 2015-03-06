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

	[Lifter::ToteScore] = {
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

void LiftManager::EnableManual(const bool mode)
{
	if(manual == false && mode == true)
		holder.retract();
	manual = mode;
}

const bool LiftManager::OffsetTarget(const double inches)
{
	if(!manual)
		return false;
	lifter.offsetTarget(inches);
	ExecuteCurrent();
	return true;
}

const bool LiftManager::ExecuteCurrent()
{
	auto state = ResolveCurrentState();

	if(manual)
	{
		SetHeightTarget(state.lifterState);	// When thrown back into automatic it will know the closest state
		return false;
	}

	bool ret;
	//std::cout<<"current state: ("<<currentState.lifterState<<", "<<currentState.holderState<<")"<<std::endl;

	if(state.lifterState == Lifter::TRANSITION || state.holderState == Holder::TRANSITION)
	{
		return false;
	}

	currentState = state;

	if(funcs[currentState.lifterState][currentState.holderState] == NULL)
	{
		ret = false;
	}
	else
	{
		ret = (this->*(funcs[currentState.lifterState][currentState.holderState]))();
	}

	if(ret && currentRoutine != NULL)
	{
		if(currentState == targetState)	// Next state in routine
		{
			std::cout<<"Next step in routine"<<std::endl;
			if(currentRoutine->front().holderState >= Holder::HOLDER_IN)
			{
				std::cout<<"Holder State: "<<currentState.holderState<<" -> "<<currentRoutine->front().holderState<<std::endl;
				SetHolderTarget(currentRoutine->front().holderState);
			}

			if(currentRoutine->front().lifterState >= Lifter::Ground)
			{
				std::cout<<"Lifter State: "<<currentState.lifterState<<" -> "<<currentRoutine->front().lifterState<<std::endl;
				SetHeightTarget(currentRoutine->front().lifterState);
			}

			currentRoutine->pop();
			if(currentRoutine->empty())
			{
				CancelRoutine();
			}
		}
	}

	return ret;
}

void LiftManager::GoToState(const DuelState &state)
{
	holder.setTargetPosition(state.holderState);
	lifter.setTargetState(state.lifterState);
}

void LiftManager::SetHeightTarget(const Lifter::Height_t h)
{
	targetState.lifterState = h;
}

void LiftManager::SetHolderTarget(const Holder::holder_t h)
{
	targetState.holderState = h;
}

void LiftManager::CancelRoutine()
{
	if(currentRoutine != NULL)
	{
		delete currentRoutine;
		currentRoutine = NULL;
	}

	routineMode = NONE;
}

const bool LiftManager::GoToGround()
{
	CancelRoutine();
	SetHeightTarget(Lifter::Ground);
	lifter.setTargetState(Lifter::Ground);
	return true;
}

const bool LiftManager::PushToteToStack()
{
	if(routineMode == PUSH_TOTE)
		return true;
	CancelRoutine();
	/*if(currentState.lifterState != Lifter::Ground)
		return false;
	*/
	currentRoutine = new queue<DuelState>;
	currentRoutine->push(DuelState(Lifter::ToteUp, -1));
	currentRoutine->push(DuelState(-1, Holder::HOLDER_OUT));
	currentRoutine->push(DuelState(Lifter::ToteScore, -1));

	routineMode = PUSH_TOTE;
	return true;
}

const bool LiftManager::ScoreStackToStep()
{
	if(routineMode == SCORE_STEP)
		return true;
	CancelRoutine();
	currentRoutine = new queue<DuelState>;
	if(currentState.holderState == Holder::HOLDING)
		currentRoutine->push(DuelState(Lifter::ToteUp, -1));

	currentRoutine->push(DuelState(-1, Holder::HOLDER_IN));
	currentRoutine->push(DuelState(Lifter::Step, -1));

	routineMode = SCORE_STEP;
	return true;
}

const bool LiftManager::ScoreStack()
{
	if(routineMode == SCORE)
		return true;
	ScoreStackToStep();
	currentRoutine->push(DuelState(Lifter::Ground, -1));

	routineMode = SCORE;
	return true;
}

bool LiftManager::MoveHook()
{
	// Disallow hook movement in these areas
	if(targetState.holderState == Holder::HOLDER_OUT)
		targetState.holderState = currentState.holderState;

	// Make sure to move to the correct position, not surpassing StackUp on the wrong side
	if(currentState.holderState != Holder::HOLDER_IN && targetState.lifterState >= Lifter::StackUp && currentState.lifterState < Lifter::StackUp)
	{
		GoToState(DuelState(Lifter::StackUp, targetState.holderState));
	}
	else
	{
		GoToState(targetState);
	}

	return true;
}

bool LiftManager::MoveHookOrExtend()
{
	GoToState(targetState);
	return true;
}

bool LiftManager::ResolveHolder()
{
	if(currentState.holderState == Holder::HOLDING)
	{
		std::cout<<":O"<<std::endl;
		targetState.holderState = Holder::HOLDING;
	}
	else if(currentState.holderState == Holder::HOLDER_OUT && targetState.lifterState == currentState.lifterState)
	{
		std::cout<<":I"<<std::endl;
		targetState.holderState = Holder::HOLDER_IN;
	}

	//std::cout<<"ResolveHolder target: Lifter: "<<Lifter::GetName(targetState.lifterState)<<", Holder: "<<Holder::GetName(targetState.holderState)<<std::endl;

	GoToState(targetState);
	return true;
}

bool LiftManager::MoveHookOrRetract()
{
	if(targetState.holderState == Holder::HOLDING)
		targetState.holderState = currentState.holderState;
	GoToState(targetState);
	return true;
}

bool LiftManager::Safety()
{
	// Ensures it does not break
	if(targetState.lifterState >= Lifter::StackUp)
	{
		targetState.holderState = Holder::HOLDER_IN;
	}

	// Ensures target is accurate
	if(targetState.holderState == Holder::HOLDING)
		targetState.holderState = currentState.holderState;

	if(targetState.lifterState >= Lifter::StackUp)
		GoToState(DuelState(Lifter::StackUp, targetState.holderState));
	else
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
	std::cout<<"It\'s over"<<std::endl;
	return true;
}
