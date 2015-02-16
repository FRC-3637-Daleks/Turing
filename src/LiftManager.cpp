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
			[Holder::HOLDER_OUT] = &LiftManager::PushToteToStack,
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
	if(funcs[targetState.lifterState][targetState.holderState] == NULL)
		return false;
	else
		return (this->*(funcs[targetState.lifterState][targetState.holderState]))();
}

bool LiftManager::MoveHook()
{
	// Disallow hook movement in these areas
	if(targetState.holderState == Holder::HOLDER_OUT)
		targetState.holderState = currentState.holderState;
	return true;
}

bool LiftManager::MoveHookOrExtend()
{
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
	return true;
}

bool LiftManager::MoveHookOrRetract()
{
	if(targetState.holderState == Holder::HOLDING)
		targetState.holderState = Holder::HOLDER_OUT;
	return true;
}

bool LiftManager::PushToteToStack()
{
	// Ensures it does not break
	if(targetState.lifterState > Lifter::StackUp)
		targetState.lifterState = Lifter::StackUp;

	// Ensures target is accurate
	if(targetState.holderState == Holder::HOLDING)
		targetState.holderState = Holder::HOLDER_OUT;

	return true;
}

bool LiftManager::LiftStack()
{
	return true;
}

bool LiftManager::Death()
{
	// Cry
	return true;
}
