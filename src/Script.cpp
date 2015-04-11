/*
 * Script.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: 3637-DS-1
 */

#include <sstream>

#include "Script.h"
/*
Script::Script(const string &f): DRR::LogObject<Script>(f), file(f), current(nullptr)
{
	LogText(DRR::LEVEL_t::INIT);
}

void Script::RegisterCommand(const string &cmdStr, const ScriptCommand &cmdFn)
{
	commands[cmdStr] = cmdFn;
}

const bool Script::Load(const string &f)
{
	if(file.is_open())
		file.close();
	file.open(f);
	return file.good();
}

const bool Script::Run()
{
	if(!file.is_open())
		return false;

	if(!current)
	{
		string line;
		std::getline(file, line);
		std::stringstream ss;
		ss<<line;
		int tout;
		ss>>tout;
		if(ss.bad())
			return false;

		string cmd;
		ss>>cmd;
		if(ss.bad())
			return false;
		if(!commands.count(cmd))
			return false;

		vector<string> args;
		while(ss.good())
		{
			string arg;
			ss>>arg;
			args.push_back(arg);
		}

		std::chrono::milliseconds toutMils(tout);
		current = std::bind<bool()>(&ScriptCommand::Execute, &commands[cmd], args, toutMils);
	}
	else
	{
		if(current())
			current = nullptr;
	}

	return file.eof();
}




*/
