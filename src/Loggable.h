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

#include "Config.h"

using std::string;
using std::ostream;
using std::vector;

/** Base class for Loggable objects.
 * Derived objects should accept a bound function and store its return value in a buffer
 */
class Loggable
{
public:
	typedef enum {CONTINUE, KILL} FAIL_COMMAND;

public:
	Loggable() {};
	virtual ~Loggable() {};

public:
	/// Overriden function should record data in buffer or log it
	virtual const int Log()=0;
    virtual const int logCurrent()=0;
};

inline const int Loggable::logCurrent()
{
    return Log();
}

template<typename DATA_TYPE>
class ValueLog: public Loggable
{
public:
	static Loggable::FAIL_COMMAND continueAnyway(const DATA_TYPE v) {return Loggable::CONTINUE;};

public:
	typedef std::function<DATA_TYPE(void)> FUNC_t;
	typedef std::function<Loggable::FAIL_COMMAND(DATA_TYPE)> LOG_EXTENSION_t;

private:
	FUNC_t fn;		///< Function called by Log which returns the value being logged.
	LOG_EXTENSION_t check;					///< Is called at every Log, defined by client
	DATA_TYPE prev;		///< Previous value called in Log

protected:
	const DATA_TYPE getPrev() const {return prev;};


public:
	/** Constructs from an output stream, function object, and flush value
	 */
	ValueLog(const FUNC_t &f, LOG_EXTENSION_t c=continueAnyway):
		fn(f), check(c) {};
	virtual ~ValueLog() {};

public:
	/// Pushes a value to buf and logs current buffer if full capacity
	const int Log() override
	{
		prev = fn();
		if(check(prev) == KILL)
			return -1;
		return 0;
	}

	const int logCurrent() override {return Loggable::logCurrent();};

public:
	FUNC_t makeExtension() const
	{
		return std::bind(&ValueLog<DATA_TYPE>::getPrev, this);
	}
};

template<typename DATA_TYPE>
class StreamLog: public ValueLog<DATA_TYPE>
{
private:
	ostream &out;
	vector<DATA_TYPE> buf;					///< Buffer of values returned by fn
	unsigned int flushFrames;				///< Number of values buffered before it flushes to output

public:
	StreamLog(ostream &o, const typename ValueLog<DATA_TYPE>::FUNC_t &f, const unsigned int flushVal,
			typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t c=ValueLog<DATA_TYPE>::continueAnyway):
			ValueLog<DATA_TYPE>(f, c), out(o), flushFrames(flushVal)
	{
		buf.reserve(flushFrames);
		out.precision(FLOATING_POINT_SIG_FIGS);
	};
	virtual ~StreamLog() {logCurrent();};

public:
	const int Log() override
	{
		const int ret = ValueLog<DATA_TYPE>::Log();

		buf.push_back(ValueLog<DATA_TYPE>::getPrev());
		if(buf.size() == flushFrames)
			return logCurrent();

		return ret;
	}

	/// Logs the current buffer immediately
	virtual const int logCurrent() override
	{
		if(buf.size() == 0)
		    return 0;
		for(auto i = buf.begin(); i != buf.end(); i++) out<<*i<<std::endl;
		buf.clear();
		out.flush();  // Ensures output stream is writing to file
		return 0;
	}
};

using NumericLog = ValueLog<double>;
using BooleanLog = ValueLog<bool>;

#endif /* SRC_LOGGABLE_H_ */
