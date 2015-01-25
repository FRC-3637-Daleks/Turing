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

public:
	static Loggable::FAIL_COMMAND checkThreshold (const CALL_BACK_t &callback, const THRESHOLD_CHECK_t &threshold, const DATA_TYPE val)
	{
		if(threshold(val))
			return callback(val);

		return Loggable::CONTINUE;
	};

	/// Converts to a std::function
	static typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t Make(const CALL_BACK_t &cb, const THRESHOLD_CHECK_t &tc)
	{
		return std::bind(&WatchLog<DATA_TYPE>::checkThreshold, cb, tc, std::placeholders::_1);
	};

};

template<typename DATA_TYPE>
inline typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t MakeWatchLog(const typename WatchLog<DATA_TYPE>::CALL_BACK_t &cb, const typename WatchLog<DATA_TYPE>::THRESHOLD_CHECK_t &tc)
{
	return WatchLog<DATA_TYPE>::Make(cb, tc);
}

#endif /* SRC_WATCH_LOGS_H_ */
