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
	typedef std::function<DATA_TYPE(void)> FUNC_t;
private:
	FUNC_t fn;		///< Function called by Log which returns the value being logged.
	vector<DATA_TYPE> buf;					///< Buffer of values returned by fn
	unsigned int flushFrames;				///< Number of values buffered before it flushes to output

public:
	/** Constructs from an output stream, function object, and flush value
	 */
	ValueLog(ostream &o, const FUNC_t &f, const unsigned int flushVal):
		Loggable(o), fn(f), flushFrames(flushVal) {buf.reserve(flushFrames);};
	virtual ~ValueLog() {logCurrent();};

public:
	/// Pushes a value to buf and logs current buffer if full capacity
	const int Log() override
	{
		buf.push_back(fn());
		if(buf.size() == flushFrames)
			return logCurrent();
		return 0;
	}

	/// Logs the current buffer immediately
	const int logCurrent() override
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
