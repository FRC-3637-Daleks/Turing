/*
 * LogServices.h
 *
 *  Created on: Jan 18, 2015
 *      Author: edward
 */

#ifndef SRC_LOGSERVICES_H_
#define SRC_LOGSERVICES_H_

#include <sstream>

#include "Loggable.h"
#include "Task.h"

using std::vector;

/** Pure virtual base class for managing logging
 * Holds a vector of log objects. Derived classes should maintain data streams
 */

class LogService
{
private:
	vector<Loggable *> logObjects;
	int framesUntilWrite;
	Task thread;

public:
	LogService(const char * threadName, const int frames=20):
		framesUntilWrite(frames), thread(threadName, FUNCPTR(&LogService::LogAll)) {thread.Start(uint32_t(this));};
	virtual ~LogService();

protected:
	virtual ostream& makeLogStream(const string &file)=0;
	virtual const int logCurrent()=0;

public:
	virtual void createLogDir(const string &command)=0;

	template<class SUBSYSTEM_CLASS>
	void addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file);

	void addNumericLog(std::function<double(void)> func, const string &file);

	template<class SUBSYSTEM_CLASS>
	void addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file);

	void addBooleanLog(std::function<bool(void)> func, const string &file);

	virtual void logText(const string &text)=0;
	virtual ostream& logText()=0;

	virtual const int LogAll();
};

inline void LogService::addNumericLog(std::function<double(void)> func, const string &file)
{
	logObjects.push_back(new NumericLog(makeLogStream(file), func, framesUntilWrite*sizeof(double)));
}

template<class SUBSYSTEM_CLASS>
void LogService::addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	addNumericLog(std::bind(func, obj), file);
}

inline void LogService::addBooleanLog(std::function<bool(void)> func, const string &file)
{
	logObjects.push_back(new BooleanLog(makeLogStream(file), func, framesUntilWrite/8));
}

template<class SUBSYSTEM_CLASS>
void LogService::addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	addBooleanLog(std::bind(func, obj), file);
}

using std::ofstream;
using std::stringstream;
using std::endl;

class FileLogger: public LogService
{
private:
	vector<ofstream*> outStreams;
	ofstream stateOut;
	stringstream doubleBuffer[2];
	bool writer;

public:
	FileLogger(const string &file, const string &command);
	virtual ~FileLogger();

public:
	void createLogDir(const string &command) override;
	virtual void logText(const string &text) override;
	virtual ostream& logText() override;

protected:
	virtual ofstream& makeLogStream(const string &file) override;
	virtual const int logCurrent() override;
};



#endif /* SRC_LOGSERVICES_H_ */
