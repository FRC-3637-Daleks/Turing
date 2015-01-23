/*
 * LogServices.h
 *
 *  Created on: Jan 18, 2015
 *      Author: edward
 */

#ifndef SRC_LOGSERVICES_H_
#define SRC_LOGSERVICES_H_

#include <sstream>
#include <thread>

#include "Config.h"
#include "Loggable.h"

using std::vector;
using std::thread;
using std::ofstream;
using std::stringstream;
using std::endl;

/** Pure virtual base class for managing logging
 * Holds a vector of log objects. Derived classes should maintain data streams
 */

class LogService
{
private:
	static void LoggingThread(LogService * const ls);

private:
	vector<Loggable *> logObjects;
	unsigned int framesUntilWrite;
	unsigned int frames;
	thread logThread;
	bool running;

public:
	LogService(const char * threadName, const unsigned int frames=DEFAULT_BUFFER_FRAMES):
		framesUntilWrite(frames), frames(0), logThread(&LogService::LoggingThread, this), running(true) {logThread.detach();};
	virtual ~LogService();

protected:
	virtual ostream& makeLogStream(const string &file)=0;
	virtual const int logCurrent()=0;

public:
	const bool isRunning() const {return running;};

	const unsigned int currentFrame() const {return frames;};

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
	logText()<<"[INFO] New Numeric Log stream opened at "<<file<<endl;
}

template<class SUBSYSTEM_CLASS>
void LogService::addNumericLog(SUBSYSTEM_CLASS * const obj, double (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	addNumericLog(std::bind(func, obj), file);
}

inline void LogService::addBooleanLog(std::function<bool(void)> func, const string &file)
{
	logObjects.push_back(new BooleanLog(makeLogStream(file), func, framesUntilWrite/8));
	logText()<<"[INFO] New Boolean Log stream opened at "<<file<<endl;
}

template<class SUBSYSTEM_CLASS>
void LogService::addBooleanLog(SUBSYSTEM_CLASS * const obj, bool (SUBSYSTEM_CLASS::*func)(), const string &file)
{
	addBooleanLog(std::bind(func, obj), file);
}

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

public:
	virtual const int LogAll() override;

protected:
	virtual ofstream& makeLogStream(const string &file) override;
	virtual const int logCurrent() override;
};

/*
class BinaryFileLogger: public FileLogger
{
public:
	BinaryFileLogger(const string &file, const string &command): FileLogger(file, command) {};
	virtual ~BinaryFileLogger() {};

protected:
	virtual ostream& makeLogStream(const string &file) override;
};
*/

#endif /* SRC_LOGSERVICES_H_ */
