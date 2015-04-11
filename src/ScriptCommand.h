/*
 * ScriptCommand.h
 *
 *  Created on: Apr 10, 2015
 *      Author: 3637-DS-1
 */

#ifndef SRC_SCRIPTCOMMAND_H_
#define SRC_SCRIPTCOMMAND_H_

#include <chrono>
#include <functional>
#include <vector>
#include <string>

using std::vector;
using std::string;

class ScriptCommand
{
private:
	typedef std::chrono::time_point<std::chrono::system_clock> sys_time;
	typedef std::function<bool(const vector<string> &)> FUNCTION_t;

private:
	FUNCTION_t fn;
	sys_time timeout;
	bool executing;

private:
	static const sys_time getNow() {return sys_time::clock::now();};

public:
	ScriptCommand(const FUNCTION_t &f): fn(f), timeout(getNow()), executing(false) {};
	ScriptCommand(const ScriptCommand& other): fn(other.fn), timeout(other.timeout), executing(other.executing) {};
	virtual ~ScriptCommand() {};

public:
	const bool Execute(const vector<string> &args, const std::chrono::milliseconds tOut);
	void Reset() {timeout = getNow();};
	const bool isExecuting() {return executing;};
	const bool isNull() {return fn == nullptr;};
};



#endif /* SRC_SCRIPTCOMMAND_H_ */
