/*
 * Watch_Logs.h
 *
 *  Created on: Jan 24, 2015
 *      Author: edward
 *    Comments: WATCH UNDERSCORE LOGS. If you say the word watch a bunch of times you start to forget what it means
 */

#ifndef SRC_WATCH_LOGS_H_
#define SRC_WATCH_LOGS_H_

#include "Loggable.h"

template<typename DATA_TYPE>
class WatchLog
{
public:
	typedef std::function<Loggable::FAIL_COMMAND(DATA_TYPE)> CALL_BACK_t;
	typedef std::function<bool(DATA_TYPE)> THRESHOLD_CHECK_t;

private:
	CALL_BACK_t callback;
	THRESHOLD_CHECK_t threshold;

public:
	WatchLog(const CALL_BACK_t &cb, const THRESHOLD_CHECK_t &tc): callback(cb), threshold(tc) {};
	WatchLog(const WatchLog& other): callback(other.callback), threshold(other.threshold) {};

public:
	Loggable::FAIL_COMMAND operator() (const DATA_TYPE val)
	{
		if(threshold(val))
			return callback(val);

		return Loggable::CONTINUE;
	};

	/// Converts to a std::function
	operator typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t () {return std::bind(&WatchLog::operator(), this, std::placeholders::_1);};

};



#endif /* SRC_WATCH_LOGS_H_ */
