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

using std::vector;

/** Pure virtual base class for managing logging
 * Holds a vector of log objects. Derived classes should maintain data streams
 */

class LogService
{
private:
	vector<Loggable *> logObjects;
	int framesUntilWrite;

public:
	LogService(const int frames=20): framesUntilWrite(frames) {};
	virtual ~LogService();

protected:
	virtual ostream& makeLogStream(const string &file)=0;
	virtual const int logCurrent()=0;

public:
	template<class SUBSYSTEM_CLASS>
	void addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file);

	template<class SUBSYSTEM_CLASS>
	void addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file);

	virtual void logText(const string &text)=0;

	virtual const int LogAll();
};

template<class SUBSYSTEM_CLASS>
void LogService::addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	logObjects.push_back(new NumericLog(makeLogStream(file), std::bind(func, obj), framesUntilWrite*sizeof(double)));
}

template<class SUBSYSTEM_CLASS>
void LogService::addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	logObjects.push_back(new BooleanLog(makeLogStream(file), std::bind(func, obj), framesUntilWrite/8));
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
	FileLogger(const string file): stateOut(file, std::ios_base::out), writer(true) {};
	virtual ~FileLogger();

public:
	virtual void logText(const string &text) override;

protected:
	virtual ofstream& makeLogStream(const string &file) override;
	virtual const int logCurrent() override;
};



#endif /* SRC_LOGSERVICES_H_ */
