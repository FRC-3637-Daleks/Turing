/*
 * Loggable.h
 *
 *  Created on: Jan 17, 2015
 *      Author: edward
 */

#ifndef SRC_LOGGABLE_H_
#define SRC_LOGGABLE_H_

#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include "WPILib.h"

using std::string;
using std::ostream;
using std::vector;

/** Base class for Loggable objects.
 * Derived objects should accept a bound function and store its return value in a buffer
 */
class Loggable
{
protected:
	ostream &out;

public:
	Loggable(ostream& o): out(o) {};
	virtual ~Loggable() {};

public:
	/// Overriden function should record data in buffer or log it
	virtual const int Log()=0;
};

class NumericLog: public Loggable
{
private:
	std::function<double(void)> fn;		///< Function called by Log which returns the double being logged.
	vector<double> buf;					///< Buffer of values returned by fn
	unsigned int flushFrames;			///< Size at which buf is emptied and sent to be logged to file

public:
	/** Constructs from an output stream, function object, and flush value
	 * Should be invoked as such
	 * NumericLog myLogObj(std::ofstream("filename", std::ios_base::binary), std::bind(ObjectType::functionName, &myObject), 10)
	 */
	NumericLog(ostream &o, auto f, const int flushVal=30):
		Loggable(o), fn(f), flushFrames(flushVal) {buf.reserve(flushFrames);};
	virtual ~NumericLog() {logCurrent();};

public:
	/// Pushes a value to buf and logs current buffer if full capacity
	const int Log() override;

	/// Logs whatever is in buf
	const int logCurrent();
};

class BooleanLog: public Loggable
{
private:
	std::function<bool(void)> fn;		///< Function called by Log which returns the bool being logged.
	vector<bool> buf;					///< Buffer of groups of 8 values returned by fn
	unsigned int flushFrames;			///< Size at which buf is emptied and sent to be logged to file

public:
	/** Constructs from an output stream, function object, and flush value
	 * Should be invoked as such
	 * BooleanLog myLogObj(std::ofstream("filename", std::ios_base::binary), std::bind(ObjectType::functionName,&myObject), 4)
	 */
	BooleanLog(ostream &o, auto f, const int flushVal=30):
		Loggable(o), fn(f), flushFrames(flushVal) {buf.reserve(flushFrames);};
	virtual ~BooleanLog() {logCurrent();};

public:
	/// Pushes a value to buf and logs current buffer if full capacity
	const int Log() override;

private:
	/// Logs whatever is in buf
	const int logCurrent();
};

#endif /* SRC_LOGGABLE_H_ */
