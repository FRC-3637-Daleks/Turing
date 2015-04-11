/*
 * ScriptCommand.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: 3637-DS-1
 */

#include "ScriptCommand.h"

const bool ScriptCommand::Execute(const vector<string> &args, const std::chrono::milliseconds tOut)
{
	if(getNow() > timeout+tOut)
	{
		executing = false;
		return true;
	}

	executing = true;
	return fn(args);
}

