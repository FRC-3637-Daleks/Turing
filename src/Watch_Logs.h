/*
 * Watch_Logs.h
 *
 *  Created on: Jan 24, 2015
 *      Author: edward
 *    Comments: WATCH UNDERSCORE LOGS. If you say the word watch a bunch of times you start to forget what it means
 */

#ifndef SRC_WATCH_LOGS_H_
#define SRC_WATCH_LOGS_H_

#include "WPILib.h"
#include "Loggable.h"

template<typename DATA_TYPE>
typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t AddSmartDashExtension(const string key, typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t ext=ValueLog<DATA_TYPE>::continueAnyway)
{
	return [key, ext](DATA_TYPE val) {
		std::stringstream toString;
		toString<<val;
		SmartDashboard::PutString(key, toString.str());
		return ext(val);
	};
}

template<>
inline typename ValueLog<double>::LOG_EXTENSION_t AddSmartDashExtension<double>(const string key, typename ValueLog<double>::LOG_EXTENSION_t ext)
{
	return [key, ext](double val) {
		SmartDashboard::PutNumber(key, val);
		return ext(val);
	};
}

template<>
inline typename ValueLog<long>::LOG_EXTENSION_t AddSmartDashExtension<long>(const string key, typename ValueLog<long>::LOG_EXTENSION_t ext)
{
	return AddSmartDashExtension<double>(key, ext);
}

template<>
inline typename ValueLog<bool>::LOG_EXTENSION_t AddSmartDashExtension<bool>(const string key, typename ValueLog<bool>::LOG_EXTENSION_t ext)
{
	return [key, ext](bool val) {
		SmartDashboard::PutBoolean(key, val);
		return ext(val);
	};
}

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
typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t MakeWatchLog(const typename WatchLog<DATA_TYPE>::CALL_BACK_t &cb, const typename WatchLog<DATA_TYPE>::THRESHOLD_CHECK_t &tc)
{
	return WatchLog<DATA_TYPE>::Make(cb, tc);
}

template<typename DATA_TYPE>
typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t MakeUpperLimit(const typename WatchLog<DATA_TYPE>::CALL_BACK_t &cb, const DATA_TYPE dt)
{
	return MakeWatchLog(cb, [dt](DATA_TYPE val) {return val > dt;});
}

template<typename DATA_TYPE>
typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t MakeLowerLimit(const typename WatchLog<DATA_TYPE>::CALL_BACK_t &cb, const DATA_TYPE dt)
{
	return MakeWatchLog(cb, [dt](DATA_TYPE val) {return val < dt;});
}

template<typename DATA_TYPE>
typename ValueLog<DATA_TYPE>::LOG_EXTENSION_t MakeValidRange(const typename WatchLog<DATA_TYPE>::CALL_BACK_t &cb, const DATA_TYPE upper, const DATA_TYPE lower)
{
	return MakeWatchLog(cb, [upper, lower](DATA_TYPE val) {return val > upper || val < lower;});
}

#endif /* SRC_WATCH_LOGS_H_ */
