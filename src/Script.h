/*
 * Script.h
 *
 *  Created on: Apr 10, 2015
 *      Author: 3637-DS-1
 */
/*
#ifndef SRC_SCRIPT_H_
#define SRC_SCRIPT_H_

#include <unordered_map>
#include <fstream>

#include "ScriptCommand.h"
#include "LogObject.h"

using std::unordered_map;
using std::ifstream;

class Script: public DRR::LogObject<Script>
{
private:
	unordered_map<string, ScriptCommand> commands;
	ifstream file;
	std::function<bool()> current;

public:
	Script(const string &f);
	virtual ~Script() {};

public:
	void RegisterCommand(const string &cmdStr, const ScriptCommand &cmdFn);
	const bool Load(const string& f);
	const bool Run();
};


#endif /* SRC_SCRIPT_H_ */

