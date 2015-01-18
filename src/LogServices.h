/*
 * LogServices.h
 *
 *  Created on: Jan 18, 2015
 *      Author: edward
 */

#ifndef SRC_LOGSERVICES_H_
#define SRC_LOGSERVICES_H_

#include "Loggable.h"

using std::vector;

/** Pure virtual base class for managing logging
 * Holds a vector of log objects. Derived classes should maintain data streams
 */

class LogService
{
private:
	vector<Loggable *> logObjects;

public:
	LogService();
	virtual ~LogService();

protected:
	virtual ostream& makeLogStream(const string &file)=0;

public:
	template<class SUBSYSTEM_CLASS>
	void addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file);

	template<class SUBSYSTEM_CLASS>
	void addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file);

	virtual const int LogAll();
};

template<class SUBSYSTEM_CLASS>
void LogService::addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	logObjects.push_back(new NumericLog(makeLogStream(file), std::bind(func, obj)));
}

template<class SUBSYSTEM_CLASS>
void LogService::addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	logObjects.push_back(new BooleanLog(makeLogStream(file), std::bind(func, obj)));
}

using std::ofstream;

class FileLogger: public LogService
{
private:
	vector<ofstream*> outStreams;

public:
	FileLogger() {};
	virtual ~FileLogger();

protected:
	virtual ofstream& makeLogStream(const string &file) override;
};



#endif /* SRC_LOGSERVICES_H_ */
